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
  CameraBlock() {
    //position.make_identity();
    //rotation.make_identity();
  }

  void set_position(float x, float y, float z) {
    //position = translation_matrix(x,y,z);
    position = makeVector3<float>(x,y,z);
    // not sure this is right...
    //position = makeVector3<float>(x,y,z);
  }
  void set_orientation(float x, float y, float z, float theta) {
    float PI  = 3.14159265;
    float deg = (theta * 180) / PI;
    rotation = makeVector4<float>(x,y,z,deg);
  }

  /*
  Matrix<float,4,4> get_inverse_transform() {
    Matrix<float,4,4> tr = position * rotation;
    tr.inverse();
    return tr;
  }

  Matrix<float,4,4> get_perspective_projection() {
    Matrix<float,4,4> pp;
    pp.clear(0);
    pp(0,0) = (2*near) / (right-left);
    pp(0,2) = (right+left) / (right-left);
    pp(1,1) = (2*near) / (top-bottom);
    pp(1,2) = (top+bottom) / (top-bottom);
    pp(2,2) = -1*(far+near) / (far-near);
    pp(2,3) = (-2*far*near) / (far-near);
    pp(3,2) = -1;
    return pp;
  }
  */

  void display() {
    std::cout << "SHOWING CAMERA'S POSITION\n";
    position.display();
    std::cout << "SHOWING CAMERA'S ROTATION\n";
    rotation.display();
    //std::cout << "SHOWING CAMERA'S INVERSE TRANSFORM\n";
    //get_inverse_transform().display();
    //std::cout << "SHOWING CAMERA'S PERSPECTIVE PROJECTION\n";
    //get_perspective_projection().display();
  }
};


#endif // __CAMERA_BLOCK_H_GUARD__
