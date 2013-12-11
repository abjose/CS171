#ifndef __CAMERA_BLOCK_H_GUARD__
#define __CAMERA_BLOCK_H_GUARD__

#include <vector>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"


class CameraBlock{
private:
  Matrix<float,4,4> view_translation;
  Matrix<float,4,4> view_rotation;

public:
  // public because laziness
  float near, far;
  float left, right, top, bottom;

  // default constructor
  CameraBlock() {
    view_translation.make_identity();
    view_rotation.make_identity();
  }

  void set_position(float x, float y, float z) {
    view_translation = translation_matrix(x,y,z);
  }
  void set_orientation(float x, float y, float z, float theta) {
    view_rotation = rotation_matrix(x,y,z,theta);
  }

  Matrix<float,4,4> get_inverse_transform() {
    Matrix<float,4,4> tr = view_translation * view_rotation;
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

  void display() {
    std::cout << "SHOWING CAMERA'S VIEW TRANSLATION\n";
    view_translation.display();
    std::cout << "SHOWING CAMERA'S VIEW ROTATION\n";
    view_rotation.display();
    std::cout << "SHOWING CAMERA'S INVERSE TRANSFORM\n";
    get_inverse_transform().display();
    std::cout << "SHOWING CAMERA'S PERSPECTIVE PROJECTION\n";
    get_perspective_projection().display();
  }
};


#endif // __CAMERA_BLOCK_H_GUARD__
