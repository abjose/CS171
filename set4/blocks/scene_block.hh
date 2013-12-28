#ifndef __SCENE_BLOCK_H_GUARD__
#define __SCENE_BLOCK_H_GUARD__

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/canvas.hh"
#include "camera_block.hh"
#include "light_block.hh"
#include "separator_block.hh"

class SceneBlock {
public:
  //private:
  int shading;
  std::shared_ptr<CameraBlock> camera;
  std::vector<std::shared_ptr<LightBlock> > lights;
  std::vector<std::shared_ptr<SeparatorBlock> > sep_list;

public:
  void set_camera(std::shared_ptr<CameraBlock> c) {
    camera = c;
  }
  void add_light(std::shared_ptr<LightBlock> l) {
    lights.push_back(l);
  }
  void add_separator(std::shared_ptr<SeparatorBlock> s) {
    sep_list.push_back(s);
  }

  //void render(std::shared_ptr<Canvas> c, int shading_type) {
  void render() {
    //Matrix<float,4,4> p_proj = camera->get_perspective_projection();
    //Matrix<float,4,4> inv_cam = camera->get_inverse_transform();
    
    for(auto &sep: sep_list) {
      glPushMatrix();
      //sep->verts_object_to_world();
      //sep->norms_object_to_world();

      // TODO: try copying something you know works here instead
      sep->render();


      // TODO: update function calls to not use tons of arguments
      // TODO: if normals don't seem right, just...push and pop stuff
      //       so can do normals correctly. Maybe worth just modifying
      //       verts_object_to_world();??
      
      // reset transform for next seperator
      glPopMatrix();
    }
  }
  
  void display() {
    std::cout << "DISPLAYING SCENE'S CAMERA: \n";
    camera->display();
    std::cout << "DISPLAYING SCENE'S LIGHTS: \n";
    for (auto &l: lights)
      l->display();
    std::cout << "DISPLAYING SCENE'S SEPARATORS\n";
    for (auto &it: sep_list) {
      it->display();
      std::cout << std::endl;
    } 
  }
};




#endif // __SCENE_BLOCK_H_GUARD__
