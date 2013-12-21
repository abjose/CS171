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
  // modify so can take a list of lights
  std::shared_ptr<LightBlock> light;
  std::vector<std::shared_ptr<SeparatorBlock> > sep_list;

public:
  void set_camera(std::shared_ptr<CameraBlock> c) {
    camera = c;
  }
  void set_light(std::shared_ptr<LightBlock> l) {
    light = l;
  }
  void add_separator(std::shared_ptr<SeparatorBlock> s) {
    sep_list.push_back(s);
  }

  void render() {//Canvas &c) {
    // ADD Z-BUFFER
    // ...hmm, could pass reference to z-buffer to rasterization thing?

    // make z-buffer
    //Canvas z_buff(c);
    //z_buff.clear(std::numeric_limits<int>::max());

    // OK, so I guess the way to switch between methods is to keep this
    // render function here, but it's called with a number representing which
    // thing to use, and then it calls render in all the sep blocks with 
    // the number as well, and then they have switch statements which will
    // run the actual thing (included from lighting.hh).

    Matrix<float,4,4> p_proj = camera->get_perspective_projection();
    Matrix<float,4,4> inv_cam = camera->get_inverse_transform();
    for(auto &sep: sep_list) {
      // get each separator to get it's final list of points
      sep->transform_vertices(p_proj, inv_cam);
      // then get each separator to rasterize the polygons on the canvas
      //sep->render(c);
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
