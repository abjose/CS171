#ifndef __SEPARATOR_BLOCK_H_GUARD__
#define __SEPARATOR_BLOCK_H_GUARD__

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"
#include "../matrix/canvas.hh"
#include "../lighting.hh"
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
    // gross
    //std::cout << "MEOW\n" << transform->get_final_transform() << std::endl;
    transform->combine_transform(t);
    transform_sans_trans->combine_transform_sans_trans(t);
    //std::cout << "MEOW2\n" << transform->get_final_transform() << std::endl;
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
	// should probably test that there aren't < 3 vertices...
	int base = temp_poly[0];
	for(int i=2; i <= temp_poly.size(); i++) {
	  int vs[] = {base, temp_poly[i], temp_poly[i-1]};
	  std::vector<int> tri (vs, vs + sizeof(vs)/sizeof(int));
	  poly_list.push_back(tri);
	}
      } else {
	// should experiment with this -- see if all this copying is necessary
	// can technically get rid of this if above works
	std::vector<int> new_poly = temp_poly;
	poly_list.push_back(new_poly);
	temp_poly.clear();
      }
    } else {
      temp_poly.push_back(vertex);
    }
  }
  void add_normal_index(int normal) {
    // DOES THIS NEED TO BE 'TRIANGULATED' IN SOME WAY TOO?
    // don't think so...
    // actually, do think so...but will only be a problem when everything
    // isn't already a triangle
    if (normal == -1) {
      // should experiment with this -- see if all this copying is necessary
      std::vector<int> new_poly_normal = temp_poly_normal;
      poly_normal_list.push_back(new_poly_normal);
      temp_poly_normal.clear();
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
  void verts_world_to_camera(Matrix<float,4,4> inv_cam) {
    std::vector<Matrix<float,3,1> > final_vertices;
    for (auto &it: vertex_list) {
      // need to homogenize...each time?
      auto v = makeVector4<float>(it[0], it[1], it[2], 1.0);
      v = (inv_cam * v).homogenize();
      final_vertices.push_back(makeVector3<float>(v[0],v[1],v[2]));
    }    
    vertex_list = final_vertices;
  }
  void verts_world_to_NDC(Matrix<float,4,4> persp_proj) {
    std::vector<Matrix<float,3,1> > final_vertices;
    for (auto &it: vertex_list) {
      // need to homogenize...each time?
      auto v = makeVector4<float>(it[0], it[1], it[2], 1.0);
      v = (persp_proj * v).homogenize();
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
      auto n = makeVector4<float>(it[0], it[1], it[2], 1.0);
      n = (final_transform_sans_trans * n).normalize();
      final_normals.push_back(makeVector3<float>(n[0],n[1],n[2]));
      //std::cout << n << std::endl;
    }
    normal_list = final_normals;
  }
  //need?
  void norms_world_to_camera(Matrix<float,4,4> inv_cam) {
    std::vector<Matrix<float,3,1> > final_normals;
    for (auto &it: normal_list) {
      auto n = makeVector4<float>(it[0], it[1], it[2], 1.0);
      n = (inv_cam * n).normalize();
      final_normals.push_back(makeVector3<float>(n[0],n[1],n[2]));
    }
    normal_list = final_normals;
  }
  //need?
  void norms_world_to_NDC(Matrix<float,4,4> persp_proj) {
    std::vector<Matrix<float,3,1> > final_normals;
    for (auto &it: normal_list) {
      auto n = makeVector4<float>(it[0], it[1], it[2], 1.0);
      n = (persp_proj * n).normalize();
      final_normals.push_back(makeVector3<float>(n[0],n[1],n[2]));
    }
    normal_list = final_normals;
  }

  void cull_backfaces(Matrix<float,4,4> persp_proj,
		      Matrix<float,4,4> inv_cam) {
    // polygons should all be triangles
    // note: this will be backwards if polygons aren't ccw - try reversing
    // if nothing shows
    std::vector<std::vector<int> > culled_list; 
    Matrix<float,4,4> final_transform = persp_proj * inv_cam * 
                                        transform->get_final_transform();

    //Matrix<float,3,1> v0p,v1p,v2p;
    Matrix<float,3,1> v0,v1,v2, a,b;
    Matrix<float,4,1> n;
    for (auto &it : poly_list) {
      v0 = vertex_list[it[0]];
      v1 = vertex_list[it[1]];
      v2 = vertex_list[it[2]];
      
      
      // TODO: move cross product into matrix class?
      // get cross product
      a = v2-v1; b = v0-v1;
      // copy into a vector for transforming
      float z[] = {a[1]*b[2] - a[2]*b[1], 
		   a[2]*b[0] - a[0]*b[2],
		   a[0]*b[1] - a[1]*b[0], 
		   1.0};
      n.copy(z);

      // transform into NDC
      n = final_transform * n;

      // check if n_z is positive
      if(n[2] > 0) { 
	// add triangle to "show" list (which will replace current poly list)
	culled_list.push_back(it);
      } else {
	//std::cout << "FOUND BACKFACE\n";
      }
    }

    // now replace old poly list with culled one
    poly_list = culled_list; 
  }

  void render(std::shared_ptr<Canvas> c,
	      std::vector<std::shared_ptr<LightBlock> > lights, 
	      std::shared_ptr<CameraBlock> camera) {
    // for each polygon (should all be triangles)
    assert(poly_list.size() == poly_normal_list.size());
    //for(auto &poly: poly_list) {
    for (int i=0; i < poly_list.size(); i++) {
      // call the proper shading function -- just flat for now
      /*
      flat_shading(vertex_list[poly[0]], normal_list[poly[0]],
		   vertex_list[poly[1]], normal_list[poly[1]],
		   vertex_list[poly[2]], normal_list[poly[2]],
		   material, light, camera, transform, c);
      */
      auto poly = poly_list[i];
      auto norm = poly_normal_list[i];
      flat_shading(vertex_list[poly[0]], normal_list[norm[0]],
		   vertex_list[poly[1]], normal_list[norm[1]],
		   vertex_list[poly[2]], normal_list[norm[2]],
		   material, lights, camera, transform, c);
      
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
