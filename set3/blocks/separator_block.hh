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

class SeparatorBlock {
private:
  std::unique_ptr<TransformBlock> transform;
  std::unique_ptr<TransformBlock> transform_sans_trans;
  std::unique_ptr<MaterialBlock> material;
  std::vector<Matrix<float,3,1> > vertex_list; 
  std::vector<Matrix<float,3,1> > normal_list; 
  std::vector<Matrix<float,4,1> > final_vertices;
  std::vector<std::vector<int> > poly_list; 
  std::vector<std::vector<int> > poly_normal_list; 
  std::vector<int> temp_poly;
  std::vector<int> temp_poly_normal;

public:
  // default constructor
  SeparatorBlock() {
    transform = std::unique_ptr<TransformBlock>(new TransformBlock);
    transform_sans_trans = std::unique_ptr<TransformBlock>(new TransformBlock);
  }

  void add_transform(std::unique_ptr<TransformBlock> t) {
    // gross
    Matrix<float,4,4> r = t->get_rotation();
    Matrix<float,4,4> s = t->get_scale();
    transform->combine_transform(std::move(t));
    transform_sans_trans->combine_transform_sans_trans(r,s);
  }
  void set_material(std::unique_ptr<MaterialBlock> m) {
    material = std::move(m);
  }
  void add_vertex(float x, float y, float z) {
    vertex_list.push_back(makeVector(x,y,z));
  }
  void add_normal(float x, float y, float z) {
    normal_list.push_back(makeVector(x,y,z));
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
    if (normal == -1) {
      // should experiment with this -- see if all this copying is necessary
      std::vector<int> new_poly_normal = temp_poly_normal;
      poly_normal_list.push_back(new_poly_normal);
      temp_poly_normal.clear();
    } else {
      temp_poly_normal.push_back(normal);
    }
  }

  void transform_vertices(Matrix<float,4,4> persp_proj,
			  Matrix<float,4,4> inv_cam) {
    // figure out final transform matrix and transform all vertices by it
    Matrix<float,4,4> final_transform = persp_proj * inv_cam * 
                                        transform->get_final_transform();
   
    for (auto &it: vertex_list) {
      // need to homogenize, is this right?
      Matrix<float,4,1> v;
      float a[] = {it[0], it[1], it[2], 1.0};
      v.copy(a);
      v = final_transform * v;
      v.homogenize();
      //v.display();
      final_vertices.push_back(v);
    }    
  }

  void transform_normals() {
    Matrix<float,4,4> final_transform_sans_trans 
      = transform_sans_trans->get_final_transform();
    final_transform_sans_trans.inverse();
    final_transform_sans_trans = final_transform_sans_trans.transpose();

    for (auto &it: normal_list) {
      Matrix<float,4,1> n;
      float a[] = {it[0], it[1], it[2], 1.0};
      n.copy(a);
      n = final_transform_sans_trans * n;
      // not sure if need to homogenize...shouldn't make a difference if 
      // normalizing?
      //n.homogenize();
      // definitely need to normalize
      n.normalize();
      //n.display();
      final_vertices.push_back(n);
    }
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
      float a[] = {a[1]*b[2] - a[2]*b[1], 
		   a[2]*b[0] - a[0]*b[2],
		   a[0]*b[1] - a[1]*b[0], 
		   1.0};
      n.copy(a);

      // transform into NDC
      n = final_transform * n;

      // check if n_z is positive
      if(n[2] > 0) { 
	// add triangle to "show" list (which will replace current poly list)
	culled_list.push_back(it);
      }
    }

    // now replace old poly list with culled one
    poly_list = culled_list; 
  }

  void render() {//Canvas &c) {
    // render vertices onto the canvas appropriately
    for(auto &poly: poly_list) {
      
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
