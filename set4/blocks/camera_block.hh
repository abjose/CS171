#ifndef __CAMERA_BLOCK_H_GUARD__
#define __CAMERA_BLOCK_H_GUARD__

#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"


class CameraBlock{
public:
  //private:
  // previously used view_translation and view_rotation
  Matrix<float,3,1> position;
  Matrix<float,4,1> rotation;

public:
  // public because laziness
  float near, far;
  float left, right, top, bottom;

  // default constructor
  CameraBlock() {}

  void set_position(float x, float y, float z) {
    position = makeVector3<float>(x,y,z);
  }
  void set_orientation(float x, float y, float z, float theta) {
    float PI  = 3.14159265;
    float deg = (theta * 180) / PI;
    rotation = makeVector4<float>(x,y,z,deg);
  }

  void display() {
    std::cout << "SHOWING CAMERA'S POSITION\n";
    position.display();
    std::cout << "SHOWING CAMERA'S ROTATION\n";
    rotation.display();
  }
};


#endif // __CAMERA_BLOCK_H_GUARD__
