

#include <iostream>

#include "framer.hh"


/*
Need...
- something to take keyframes, copy them if necessary for later passing on to
  interpolation thing (for the slopes), and convert rotations to quaternions
- something to take processed keyframes and interpolate, filling up a new
  vector with the actual frames
- Modify TransformBlock to having a quat?
 */



void stuff() {
  auto q  = glm::gtc::quaternion::quat(1,1,1,1);
  //auto q2 = gtx::quaternion::angleAxis(degrees(RotationAngle), RotationAxis);
  glm::core::type::mat4 m;
}


void Framer::quatify() {
  // take keyframe vector and convert rotations to quaternions
  // also copy first and last elements? (to enable better interpolation)
  
}

void Framer::framify() {
  // take quatified keyframe vector and populate the frames vector appropriately
  
}


void Framer::display() {
  for (auto &trans : frames) {
    std::cout << trans << std::endl;
  }
}
