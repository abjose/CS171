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
  std::unique_ptr<CameraBlock> camera;
  std::unique_ptr<LightBlock> light;
  std::vector<std::unique_ptr<SeparatorBlock> > sep_list;

public:
  void set_camera(std::unique_ptr<CameraBlock> c) {
    camera = std::move(c);
  }
  void set_light(std::unique_ptr<LightBlock> l) {
    light = std::move(l);
  }
  void add_separator(std::unique_ptr<SeparatorBlock> s) {
    sep_list.push_back(std::move(s));
  }

  void render(Canvas &c) {
    Matrix<float,4,4> p_proj = camera->get_perspective_projection();
    Matrix<float,4,4> inv_cam = camera->get_inverse_transform();
    for(auto &sep: sep_list) {
      // get each separator to get it's final list of points
      sep->transform_vertices(p_proj, inv_cam);
      // then get each separator to rasterize the polygons on the canvas
      sep->render(c);
    }
  }
  
  void display() {
    std::cout << "DISPLAYING SCENE'S CAMERA: \n";
    camera->display();
    std::cout << "DISPLAYING SCENE'S LIGHT: \n";
    light->display();
    std::cout << "DISPLAYING SCENE'S SEPARATORS\n";
    for (auto &it: sep_list) {
      it->display();
      std::cout << std::endl;
    } 
  }
};




#endif // __SCENE_BLOCK_H_GUARD__
