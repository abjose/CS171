#ifndef __TRANSFORM_BLOCK_H_GUARD__
#define __TRANSFORM_BLOCK_H_GUARD__

#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

class TransformBlock {
public:
  //private:
  //Matrix<float,4,1> transform;
  Matrix<float,4,1> rotation;
  Matrix<float,3,1> translation;
  Matrix<float,3,1> scale;

  //public:
  // default constructor
  TransformBlock() {}

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

  void display() {
    std::cout << "ROTATION:\n";
    rotation.display();
    std::cout << "TRANSLATION:\n";
    translation.display();
    std::cout << "SCALE:\n";
    scale.display();
  }

};


#endif // __TRANSFORM_BLOCK_H_GUARD__
