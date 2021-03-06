#ifndef __SEPARATOR_BLOCK_H_GUARD__
#define __SEPARATOR_BLOCK_H_GUARD__

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"
#include "../matrix/canvas.hh"
#include "../lighting/lighting.hh"
#include "transform_block.hh"
#include "material_block.hh"
#include "light_block.hh"
#include "camera_block.hh"

class SeparatorBlock {
private:
  std::shared_ptr<TransformBlock> transform;
  std::shared_ptr<TransformBlock> transform_sans_trans;
  std::shared_ptr<MaterialBlock> material;
  std::vector<Matrix<float,3,1> > vertex_list; 
  std::vector<Matrix<float,3,1> > normal_list; 
  std::vector<std::vector<int> > poly_list; 
  std::vector<std::vector<int> > poly_normal_list; 
  std::vector<int> temp_poly;
  std::vector<int> temp_poly_normal;


public:
  // default constructor
  SeparatorBlock() {
    transform = std::shared_ptr<TransformBlock>(new TransformBlock);
    transform_sans_trans = std::shared_ptr<TransformBlock>(new TransformBlock);
  }

  void add_transform(std::shared_ptr<TransformBlock> t) {
    transform->combine_transform(t);
    transform_sans_trans->combine_transform_sans_trans(t);
  }
  void set_material(std::shared_ptr<MaterialBlock> m) {
    material = m;
  }
  void add_vertex(float x, float y, float z) {
    vertex_list.push_back(makeVector3<float>(x,y,z));
  }
  void add_normal(float x, float y, float z) {
    normal_list.push_back(makeVector3<float>(x,y,z));
  }
  void add_poly_index(int vertex) {
    if (vertex == -1) {
      if (temp_poly.size() > 3) {
	// tringulate non-triangle polygon
	for(int i=2; i < temp_poly.size(); i++) {
	  std::vector<int> tri(3,0);// (vs, vs + sizeof(vs)/sizeof(int));
	  tri[0] = temp_poly[0];
	  tri[1] = temp_poly[i-1];
	  tri[2] = temp_poly[i];
	  poly_list.push_back(tri);
	}
	temp_poly.clear();
      } else {
	poly_list.push_back(temp_poly);
	temp_poly.clear();
      }
    } else {
      temp_poly.push_back(vertex);
    }
  }
  void add_normal_index(int normal) {
    if (normal == -1) {
      if (temp_poly_normal.size() > 3) {
	for(int i=2; i < temp_poly_normal.size(); i++) {
	  std::vector<int> tri(3,0);// (vs, vs + sizeof(vs)/sizeof(int));
	  tri[0] = temp_poly_normal[0];
	  tri[1] = temp_poly_normal[i-1];
	  tri[2] = temp_poly_normal[i];
	  poly_normal_list.push_back(tri);
	}
	temp_poly_normal.clear();
      } else {
	// should experiment with this -- see if all this copying is necessary
	std::vector<int> new_poly_normal = temp_poly_normal;
	poly_normal_list.push_back(new_poly_normal);
	temp_poly_normal.clear();
      }
    } else {
      temp_poly_normal.push_back(normal);
    }
  }


  void verts_object_to_world() {
    auto final_transform = transform->get_final_transform();
    std::vector<Matrix<float,3,1> > final_vertices;
    for (auto &it: vertex_list) {
      // need to homogenize...each time?
      auto v = makeVector4<float>(it[0], it[1], it[2], 1.0);
      v = (final_transform * v).homogenize();
      final_vertices.push_back(makeVector3<float>(v[0],v[1],v[2]));
    }    
    vertex_list = final_vertices;
  }

  void norms_object_to_world() {
    std::vector<Matrix<float,3,1> > final_normals;
    Matrix<float,4,4> final_transform_sans_trans = 
      transform_sans_trans->get_final_transform();
    final_transform_sans_trans.inverse();
    final_transform_sans_trans = final_transform_sans_trans.transpose();

    for (auto &it: normal_list) {
      auto n = makeVector4<float>(it[0], it[1], it[2], 0.);
      n = final_transform_sans_trans * n;
      final_normals.push_back(makeVector3<float>(n[0],n[1],n[2]).normalize());
    }
    normal_list = final_normals;
  }

  void cull_backfaces(Matrix<float,4,4> persp_proj,
		      Matrix<float,4,4> inv_cam) {
    // polygons should all be triangles
    // note: this will be backwards if polygons aren't ccw - try reversing
    // if nothing shows
    assert(poly_list.size() == poly_normal_list.size());
    std::vector<std::vector<int> > culled_polys;     
    std::vector<std::vector<int> > culled_norms; 
    Matrix<float,3,1> v0,v1,v2, a,b;
    Matrix<float,4,1> n;
    for (int i=0; i < poly_list.size(); i++) {
      auto poly = poly_list[i];
      auto norm = poly_normal_list[i];
      v0 = vertex_list[poly[0]];
      v1 = vertex_list[poly[1]];
      v2 = vertex_list[poly[2]];
      
      // get cross product
      a = (v1-v0).normalize(); b = (v2-v1).normalize();
      n = makeVector4<float>(a[1]*b[2] - a[2]*b[1], 
			     a[2]*b[0] - a[0]*b[2],
			     a[0]*b[1] - a[1]*b[0], 
			     0.0);
      auto test = makeVector3<float>(n[0],n[1],n[2]).normalize();

      // check if n_z is positive
      if(test[2] >= 0) { 
	// add triangle to "show" list (which will replace current poly list)
	culled_polys.push_back(poly);
	culled_norms.push_back(norm);
      } else {
	std::cerr << "found backface: ";
	for (auto& bla : poly)
	    std::cerr << bla << ", ";
	std::cerr << std::endl;
      }
    }
    // now replace old lists with culled ones
    poly_list = culled_polys; 
    poly_normal_list = culled_norms; 
  }

  void cull_backfaces_dot(Matrix<float,3,1> cam_pos) {
    // use dot product of surface normal (can just take any??)
    // and camera-to-poly vector 
    assert(poly_list.size() == poly_normal_list.size());
    std::vector<std::vector<int> > culled_polys;     
    std::vector<std::vector<int> > culled_norms; 
    Matrix<float,3,1> v0,v1,v2, avg_v;
    for (int i=0; i < poly_list.size(); i++) {
      auto poly = poly_list[i];
      auto norm = poly_normal_list[i];
      v0 = vertex_list[poly[0]];
      v1 = vertex_list[poly[1]];
      v2 = vertex_list[poly[2]];
      avg_v = (v0+v1+v2) / 3.0;

      auto cam_to_surf = (cam_pos - avg_v).normalize(); 
      auto surf_norm = normal_list[norm[0]].normalize(); // just choose one
      // TODO: also try calculating surf_norm by getting cross product of face
      //       vertices...might help figure out why other one doesn't work
      auto dot = surf_norm.dot(cam_to_surf);      
      
      // check if dot is positive
      if(dot >= 0) { 
	// add triangle to "show" list (which will replace current poly list)
	culled_polys.push_back(poly);
	culled_norms.push_back(norm);
      } else {
	std::cerr << "found backface: ";
	for (auto& bla : poly)
	    std::cerr << bla << ", ";
	std::cerr << std::endl;
      }
    }
    // now replace old lists with culled ones
    poly_list = culled_polys; 
    poly_normal_list = culled_norms; 
  }

  void render(std::shared_ptr<Canvas> c,
	      std::vector<std::shared_ptr<LightBlock> > lights, 
	      std::shared_ptr<CameraBlock> camera,
	      int shading_type) {
    assert(poly_list.size() == poly_normal_list.size());
    // for each polygon (should all be triangles)
    for (int i=0; i < poly_list.size(); i++) {
      // call the proper shading function
      auto poly = poly_list[i];
      auto norm = poly_normal_list[i];
      
      switch(shading_type) {
      case 0: // flat shading
	flat_shading(vertex_list[poly[0]], normal_list[norm[0]],
		     vertex_list[poly[1]], normal_list[norm[1]],
		     vertex_list[poly[2]], normal_list[norm[2]],
		     material, lights, camera, c);
	break;
      case 1: // gouraud shading
	gouraud_shading(vertex_list[poly[0]], normal_list[norm[0]],
			vertex_list[poly[1]], normal_list[norm[1]],
			vertex_list[poly[2]], normal_list[norm[2]],
			material, lights, camera, c);
	break;
      case 2: // phong shading
	phong_shading(vertex_list[poly[0]], normal_list[norm[0]],
		      vertex_list[poly[1]], normal_list[norm[1]],
		      vertex_list[poly[2]], normal_list[norm[2]],
		      material, lights, camera, c);
	break;
      }
    }
  }

  void display() {
    std::cout << "SHOWING SEPERATOR'S TRANSFORM\n";
    transform->display();
    std::cout << "SHOWING SEPERATOR'S TRANSFORM (no translations)\n";
    transform_sans_trans->display();
    std::cout << "SHOWING SEPERATOR'S MATERIAL\n";
    material->display();
    std::cout << "SHOWING SEPERATOR'S VERTICES\n";
    for (auto &it: vertex_list) {
      it.display();
    }
    std::cout << "SHOWING SEPERATOR'S NORMALS\n";
    for (auto &it: normal_list) {
      it.display();
    }
    std::cout << "SHOWING SEPERATOR'S POLYGONS\n";
    for (auto &it: poly_list) {
      std::cout << "Polygon: ";
      for (auto &vert: it) {
	std::cout << vert << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << "SHOWING SEPERATOR'S NORMAL SETS\n";
    for (auto &it: poly_normal_list) {
      std::cout << "Normal set: ";
      for (auto &normal: it) {
	std::cout << normal << ", ";
      }
      std::cout << std::endl;
    }
  }
};


#endif // __SEPARATOR_BLOCK_H_GUARD__
