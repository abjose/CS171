#ifndef __SEPARATOR_BLOCK_H_GUARD__
#define __SEPARATOR_BLOCK_H_GUARD__

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"
#include "transform_block.hh"
#include "material_block.hh"
#include "light_block.hh"
#include "camera_block.hh"

class SeparatorBlock {
public:
  //private:
  std::vector<std::shared_ptr<TransformBlock> > transforms;
  std::shared_ptr<MaterialBlock> material;
  std::vector<Matrix<float,3,1> > vertex_list; 
  std::vector<Matrix<float,3,1> > normal_list; 
  std::vector<Matrix<float,2,1> > texture_list; 
  std::vector<std::vector<int> > poly_list; 
  std::vector<std::vector<int> > poly_normal_list;
  std::vector<std::vector<int> > poly_texture_list;
  std::vector<int> temp_poly;
  std::vector<int> temp_poly_normal;
  std::vector<int> temp_poly_texture;
  // openGL stuff
  GLfloat *vertices, *normals;
  int n_verts;



public:
  // default constructor
  SeparatorBlock() {}

  void add_transform(std::shared_ptr<TransformBlock> t) {
    transforms.push_back(t);
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
  void add_texcoord(float x, float y) {
    texture_list.push_back(makeVector2<float>(x,y));
  }

  void add_poly_index(int vertex) {
    if (vertex == -1) {
      if (temp_poly.size() > 3) {
	// triangulate non-triangle polygon
	for(int i=2; i < temp_poly.size(); i++) {
	  std::vector<int> tri(3,0);// (vs, vs + sizeof(vs)/sizeof(int));
	  tri[0] = temp_poly[0];
	  tri[1] = temp_poly[i-1];
	  tri[2] = temp_poly[i];
	  poly_list.push_back(tri);
	}
	temp_poly.clear();
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
 void add_texture_index(int texture) {
    if (texture == -1) {
      if (temp_poly_texture.size() > 3) {
	for(int i=2; i < temp_poly_texture.size(); i++) {
	  std::vector<int> tri(3,0);// (vs, vs + sizeof(vs)/sizeof(int));
	  tri[0] = temp_poly_texture[0];
	  tri[1] = temp_poly_texture[i-1];
	  tri[2] = temp_poly_texture[i];
	  poly_texture_list.push_back(tri);
	}
	temp_poly_texture.clear();
      } else {
	// should experiment with this -- see if all this copying is necessary
	std::vector<int> new_poly_texture = temp_poly_texture;
	poly_texture_list.push_back(new_poly_texture);
	temp_poly_texture.clear();
      }
    } else {
      temp_poly_texture.push_back(texture);
    }
  }

  void init_material() {
    GLfloat emit[] = {0.0, 0.0, 0.0, 1.0}; // wat
    GLfloat  amb[] = {material->ambient[0],
		      material->ambient[1],
		      material->ambient[2]};
    GLfloat  diff[] = {material->diffuse[0],
		       material->diffuse[1],
		       material->diffuse[2]};
    GLfloat  spec[] = {material->specular[0],
		       material->specular[1],
		       material->specular[2]};
    GLfloat shiny = material->shininess;

    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_EMISSION, emit);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shiny);
  }

  void render() {
    assert(poly_list.size() == poly_normal_list.size());

    // init vertex/normal arrays
    n_verts = 3*poly_list.size();  // 3 vertices per face
    int n_floats = 3*n_verts;  // 3 floats per vertex

    vertices = new GLfloat[n_floats];
    normals = new GLfloat[n_floats];

    // TODO: considering these arrays are always the same, should just
    // calculate them once?
    // for each polygon (should all be triangles)
    for (int i=0; i < poly_list.size(); i++) {
      auto poly = poly_list[i];
      auto norm = poly_normal_list[i];

      // insert vertices and normals into arrays for rendering
      for (int v=0; v<3; v++) {
	for (int f=0; f<3; f++) {       	  
	  vertices[9*i+3*v+f] = vertex_list[poly[v]][f];
	  normals[9*i+3*v+f]  = normal_list[norm[v]][f];
	}	
      }
    }    
  }

  void display() {
    std::cout << "SHOWING SEPERATOR'S TRANSFORMS\n";
    for (auto& t : transforms)
      t->display();
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
