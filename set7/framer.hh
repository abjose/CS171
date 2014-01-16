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

  void quatify();
  void framify();

public:
  std::vector<std::shared_ptr<KeyframeBlock> > frames;

  // default constructor
  Framer(std::vector<std::shared_ptr<KeyframeBlock> > k) {
    keyframes = k;
  }


  void display();

};










#endif // __FRAMER_H_GUARD__
