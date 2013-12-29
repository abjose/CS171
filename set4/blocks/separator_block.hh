#ifndef __SEPARATOR_BLOCK_H_GUARD__
#define __SEPARATOR_BLOCK_H_GUARD__

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"
#include "../matrix/canvas.hh"
#include "transform_block.hh"
#include "material_block.hh"
#include "light_block.hh"
#include "camera_block.hh"

class SeparatorBlock {
private:
  std::vector<std::shared_ptr<TransformBlock> > transforms;
  //std::shared_ptr<TransformBlock> transform_sans_trans;
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
    //transform = std::shared_ptr<TransformBlock>(new TransformBlock);
    //transform_sans_trans = std::shared_ptr<TransformBlock>(new TransformBlock);
  }

  void add_transform(std::shared_ptr<TransformBlock> t) {
    transforms.push_back(t);
    //transform->combine_transform(t);
    //transform_sans_trans->combine_transform_sans_trans(t);
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
	// triangulate non-triangle polygon
	for(int i=2; i < temp_poly.size(); i++) {
	  std::vector<int> tri(3,0);// (vs, vs + sizeof(vs)/sizeof(int));
	  tri[0] = temp_poly[0];
	  tri[1] = temp_poly[i-1];
	  tri[2] = temp_poly[i];
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
      if (temp_poly_normal.size() > 3) {
	for(int i=2; i < temp_poly_normal.size(); i++) {
	  std::vector<int> tri(3,0);// (vs, vs + sizeof(vs)/sizeof(int));
	  tri[0] = temp_poly_normal[0];
	  tri[1] = temp_poly_normal[i-1];
	  tri[2] = temp_poly_normal[i];
	  poly_normal_list.push_back(tri);
	}
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


  /*
  void verts_object_to_world() {
    auto final_transform = transform->get_final_transform();
    std::vector<Matrix<float,3,1> > final_vertices;
    for (auto &it: vertex_list) {
      //std::cout << "In verts_object_to_world...converting...\n";
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
    }
    normal_list = final_normals;
  }
  //need?
  void norms_world_to_camera(Matrix<float,4,4> inv_cam) {
    std::vector<Matrix<float,3,1> > final_normals;
    for (auto &it: normal_list) {
      auto n = makeVector4<float>(it[0], it[1], it[2], 1.0);
      n = (inv_cam * n);//.normalize();
      final_normals.push_back(makeVector3<float>(n[0],n[1],n[2]).normalize());
      // TODO: pretty weird that when you normalize here, doesn't seem to
      //       'last' to lighting stuff (because different when normalize there)
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
  */


  void verts_object_to_world() {
    // should have already pushed
    for (auto& t : transforms) {
      // TODO: might be backwards! but is supposed to be a stack...
      // also, will you have to push and pop each time? hmm..
      // i.e. push isn't a "save state", you actually have to do it a lot
      glTranslatef(t->translation[0],
		   t->translation[1],
		   t->translation[2]);
      glRotatef(t->rotation[3],
		t->rotation[0],
		t->rotation[1],
		t->rotation[2]);
      glScalef(t->scale[0],
	       t->scale[1],
	       t->scale[2]);
    }
  }

  void norms_object_to_world() {
    for (auto& t : transforms) {
      // note - change this if change verts version
      glRotatef(t->rotation[3],
		t->rotation[0],
		t->rotation[1],
		t->rotation[2]);
      glScalef(t->scale[0],
	       t->scale[1],
	       t->scale[2]);
    }
  }


  //  void render(std::shared_ptr<Canvas> c,
  //	      std::vector<std::shared_ptr<LightBlock> > lights, 
  //	      std::shared_ptr<CameraBlock> camera,
  //	      int shading_type) {
  void render() {
    assert(poly_list.size() == poly_normal_list.size());

    // init vertex/normal arrays
    int n_verts = 3*poly_list.size();  // 3 vertices per face
    int n_floats = 3*n_verts;  // 3 floats per vertex
    GLfloat vertices[n_floats];
    GLfloat normals[n_floats];

    // TODO: considering these arrays are always the same, should just
    // calculate them once?
    // for each polygon (should all be triangles)
    for (int i=0; i < poly_list.size(); i++) {
      auto poly = poly_list[i];
      auto norm = poly_normal_list[i];
      for (int v=0; v<3; v++) {
	for (int f=0; f<3; f++) {
	  // TODO: get rid of magic numbers like 3
	  vertices[9*i+3*v+f] = vertex_list[poly[v]][f];
	  normals[9*i+3*v+f]  = normal_list[norm[v]][f];
	}
	//vertices[9*i+3*v+0] = vertex_list[poly[v]][0];
	//vertices[9*i+3*v+1] = vertex_list[poly[v]][1];
	//vertices[9*i+3*v+2] = vertex_list[poly[v]][2];
      }
    }
      
    // do you need to enable/disable every time?
    //verts_object_to_world();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glDrawArrays(GL_TRIANGLES, 0, n_verts);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }

  void display() {
    std::cout << "SHOWING SEPERATOR'S TRANSFORMS\n";
    for (auto& t : transforms)
      t->display();
    //std::cout << "SHOWING SEPERATOR'S TRANSFORM\n";
    //transform->display();
    //std::cout << "SHOWING SEPERATOR'S TRANSFORM (no translations)\n";
    //transform_sans_trans->display();
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
