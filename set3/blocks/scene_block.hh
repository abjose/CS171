#ifndef __SCENE_BLOCK_H_GUARD__
#define __SCENE_BLOCK_H_GUARD__

#include <vector>
#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/canvas.hh"
#include "camera_block.hh"
#include "light_block.hh"
#include "separator_block.hh"

class SceneBlock {
private:
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

  void render(std::shared_ptr<Canvas> c, int shading_type) {
    Matrix<float,4,4> p_proj = camera->get_perspective_projection();
    Matrix<float,4,4> inv_cam = camera->get_inverse_transform();
    for(auto &sep: sep_list) {
      //sep->cull_backfaces(p_proj, inv_cam);
      sep->verts_object_to_world();
      sep->norms_object_to_world();
      sep->cull_backfaces_dot(camera->position);
      sep->render(c, lights, camera, shading_type);
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
