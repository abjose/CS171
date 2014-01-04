#ifndef __SEPARATOR_BLOCK_H_GUARD__
#define __SEPARATOR_BLOCK_H_GUARD__

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"
#include "../matrix/canvas.hh"
#include "transform_block.hh"

class SeparatorBlock {
private:
  std::unique_ptr<TransformBlock> transform;
  std::vector<Matrix<float,3,1> > vertex_list; 
  std::vector<Matrix<float,4,1> > final_vertices;
  std::vector<std::vector<int> > poly_list; 
  std::vector<int> temp_poly;

public:
  // default constructor
  SeparatorBlock() {
    transform = std::unique_ptr<TransformBlock>(new TransformBlock);
  }

  // destructor
  ~SeparatorBlock() {}

  void add_transform(std::unique_ptr<TransformBlock> t) {
    transform->combine_transform(std::move(t));
  }
  void add_vertex(float x, float y, float z) {
    vertex_list.push_back(makeVector(x,y,z));
  }
  //void add_poly(std::vector<int> poly) {
  void add_poly_vert(int vertex) {
    if (vertex == -1) {
      // should experiment with this -- see if all this copying is necessary
      std::vector<int> new_poly = temp_poly;
      poly_list.push_back(new_poly);
      temp_poly.clear();
    } else {
      temp_poly.push_back(vertex);
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
      // set initial vert to final vertex so lines close polygon
      int prev_vert = poly[poly.size()-1];
      for(auto &curr_vert: poly) {
	Matrix<float,4,1> p0 = final_vertices[curr_vert];
	Matrix<float,4,1> p1 = final_vertices[prev_vert];
	c.scale_draw_line(p0[0], p0[1], p1[0], p1[1]);
	prev_vert = curr_vert; // heh heh heh
      }
    }
  }

  void display() {
    std::cout << "SHOWING SEPERATOR'S TRANSFORM\n";
    transform->display();
    std::cout << "SHOWING SEPERATOR'S VERTICES\n";
    for (auto &it: vertex_list) {
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
  }
};





#endif // __SEPARATOR_BLOCK_H_GUARD__
