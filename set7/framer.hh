#ifndef __FRAMER_H_GUARD__
#define __FRAMER_H_GUARD__

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <vector>

#include "blocks/keyframe_block.hh"

class Framer {
private:
  std::vector<std::shared_ptr<KeyframeBlock> > keyframes;
  int num_frames;

  void quatify();
  void framify();
  template <typename T>
  T basic_interpolate(float u, T k0, T k1);
  template <typename T>
  T interpolate(float u, T k0, T k1, T k2, T k3);

public:
  std::vector<std::shared_ptr<KeyframeBlock> > frames;

  // default constructor
  Framer(std::vector<std::shared_ptr<KeyframeBlock> > k, int f) {
    keyframes = k;
    num_frames = f;
    std::cout << "num_frames in framer: " << num_frames << std::endl;
    quatify();
    framify();
    std::cout << "Number of frames made: " << frames.size() << std::endl;
  }


  void display();

};










#endif // __FRAMER_H_GUARD__
