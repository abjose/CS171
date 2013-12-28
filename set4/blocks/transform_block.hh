#ifndef __TRANSFORM_BLOCK_H_GUARD__
#define __TRANSFORM_BLOCK_H_GUARD__

#include <memory>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

class TransformBlock {
private:
  Matrix<float,4,4> transform;
  Matrix<float,4,4> rotation;
  Matrix<float,4,4> translation;
  Matrix<float,4,4> scale;

public:
  // default constructor
  TransformBlock() {
    // init matrices to be identity in case they're never set
    transform.make_identity();
    rotation.make_identity();
    translation.make_identity();
    scale.make_identity();
  }

  void set_rotation(float x, float y, float z, float theta) {
    // ROTATIONS ARE IN DEGREES FOR OPENGL OMG
    assert(false);
    rotation = rotation_matrix(x,y,z,theta);
  }
  void set_translation(float x, float y, float z) {
    translation = translation_matrix(x,y,z);
  }
  void set_scale(float a, float b, float c) {
    scale = scale_matrix(a,b,c);
  }
  Matrix<float,4,4> get_rotation() {
    return rotation;
  }
  Matrix<float,4,4> get_scale() {
    return scale;
  }
  void combine_transform(std::shared_ptr<TransformBlock> t2) {
    transform = transform * t2->translation * t2->rotation * t2->scale;
    //std::cout << "TESTTT:\n" <<  transform << std::endl;
  }
  void combine_transform_sans_trans(std::shared_ptr<TransformBlock> t2) {
    transform = transform * t2->rotation * t2->scale;
  }

  void display() {
    std::cout << "ROTATION:\n";
    rotation.display();
    std::cout << "TRANSLATION:\n";
    translation.display();
    std::cout << "SCALE:\n";
    scale.display();
  }

  Matrix<float,4,4> get_final_transform() {
    return transform;
  }
};


#endif // __TRANSFORM_BLOCK_H_GUARD__
