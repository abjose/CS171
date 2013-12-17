#ifndef __SEPARATOR_BLOCK_H_GUARD__
#define __SEPARATOR_BLOCK_H_GUARD__

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"
#include "../matrix/canvas.hh"
#include "transform_block.hh"
#include "material_block.hh"

class SeparatorBlock {
private:
  std::unique_ptr<TransformBlock> transform;
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
  }

  // destructor
  ~SeparatorBlock() {}

  void add_transform(std::unique_ptr<TransformBlock> t) {
    //transform = std::move(t);
    //std::cout << "ADDING TRANSFORM IN SEPLIST\n";
    //transform->display();
    transform->combine_transform(std::move(t));
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
      // should experiment with this -- see if all this copying is necessary
      std::vector<int> new_poly = temp_poly;
      poly_list.push_back(new_poly);
      temp_poly.clear();
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
    
    for(auto &it: vertex_list) {
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

  void render(Canvas &c) {
    // render vertices onto the canvas appropriately
    for(auto &poly: poly_list) {
    //  std::cout << "On polygon: ";
    //  for(auto &vertex: poly) {
    //    std::cout << vertex << ", ";
    //  }
    //  std::cout << std::endl;
      
      // set initial vert to final vertex so lines close polygon
      int prev_vert = poly[poly.size()-1];
      for(auto &curr_vert: poly) {
	Matrix<float,4,1> p0 = final_vertices[curr_vert];
	Matrix<float,4,1> p1 = final_vertices[prev_vert];
	c.scale_draw_line(p0[0], p0[1], p1[0], p1[1]);
	//std::cout << "Drawing a line: (" << p0[0] << ", " << p0[2] << ") to (" << p1[0] << ", " << p1[1] << ")\n";
	prev_vert = curr_vert; // heh heh heh
      }
    }
  }

  void display() {
    std::cout << "SHOWING SEPERATOR'S TRANSFORM\n";
    transform->display();
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
