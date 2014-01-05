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
  bool is_lines;
  int shading;
  std::shared_ptr<CameraBlock> camera;
  std::vector<std::shared_ptr<LightBlock> > lights;
  std::vector<std::shared_ptr<SeparatorBlock> > sep_list;

public:
  SceneBlock() {
    is_lines = false;
  }

  void set_camera(std::shared_ptr<CameraBlock> c) {
    camera = c;
  }
  void add_light(std::shared_ptr<LightBlock> l) {
    lights.push_back(l);
  }
  void add_separator(std::shared_ptr<SeparatorBlock> s) {
    sep_list.push_back(s);
  }

  // void render() {    
  //   for(auto &sep: sep_list) {
  //     sep->init_material();

  //     glPushMatrix();
  //     sep->object_to_world();
  //     sep->render();
  //     glPopMatrix();
  //   }
  // }
  
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
