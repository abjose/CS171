#ifndef __KEYFRAME_BLOCK_H_GUARD__
#define __KEYFRAME_BLOCK_H_GUARD__

#include <memory>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

class KeyframeBlock {
public:
  //private:
  Matrix<float,4,1> rotation;
  glm::gtc::quaternion::quat quat_rot;
  Matrix<float,3,1> translation;
  Matrix<float,3,1> scale;
  int frame;

  //public:
  // default constructor
  KeyframeBlock() {}

  void set_rotation(float x, float y, float z, float theta) {
    // TODO: put PI somewhere else...
    float PI  = 3.14159265;
    float deg = (theta * 180) / PI;
    rotation = makeVector4<float>(x,y,z,deg);
  }
  void set_translation(float x, float y, float z) {
    translation = makeVector3<float>(x,y,z);
  }
  void set_scale(float a, float b, float c) {
    scale = makeVector3<float>(a,b,c);
  }

  void find_quat_rot() {
    // assumes x,y,z,theta in 'rotation'
    // pass as degrees, x,y,z
    quat_rot = glm::gtx::quaternion::angleAxis(rotation[3],
					       rotation[0],
					       rotation[1],
					       rotation[2]);
  }

  void set_frame(int f) {
    frame = f;
  }

  void display() {
    std::cout << "ROTATION:\n";
    rotation.display();
    std::cout << "TRANSLATION:\n";
    translation.display();
    std::cout << "SCALE:\n";
    scale.display();
  }

};


#endif // __KEYFRAME_BLOCK_H_GUARD__
