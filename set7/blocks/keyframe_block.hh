#ifndef __KEYFRAME_BLOCK_H_GUARD__
#define __KEYFRAME_BLOCK_H_GUARD__

#include <memory>
#include <math.h>
#include <algorithm>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

#define PI 3.14159265

class KeyframeBlock {
public:
  //private:
  Matrix<float,4,1> rotation;
  Matrix<float,4,1> quat;
  Matrix<float,3,1> translation;
  Matrix<float,3,1> scale;
  int frame;

  //public:
  // default constructor
  KeyframeBlock() {}

  void set_rotation(float x, float y, float z, float theta) {
    //float deg = (theta * 180) / PI;
    //rotation = makeVector4<float>(x,y,z,deg);
    rotation = makeVector4<float>(x,y,z,theta); // theta already in degrees
  }
  void set_translation(float x, float y, float z) {
    translation = makeVector3<float>(x,y,z);
  }
  void set_scale(float a, float b, float c) {
    scale = makeVector3<float>(a,b,c);
  }

  void find_quat() {
    // assumes x,y,z,theta in 'rotation'. Convert theta to radians.
    float half_theta = 0.5*(rotation[3]*PI / 180.0);
    float x = rotation[0]; float y = rotation[1]; float z = rotation[2];
    quat = makeVector4<float>(cos(half_theta),
			      x*sin(half_theta),
			      y*sin(half_theta),
			      z*sin(half_theta));
    quat = quat.normalize();
  }

  void find_rot() {
    // assumes quat =(s,v1,v2,v3) has been found, overwrites rotation with
    // stuff to pass to glRotate. Make sure to convert back to degrees.
    // again switching back to x,y,z,theta....lol.
    float q0 = std::min<float>(std::max<float>(-1.0, quat[0]), 1.0);
    float theta = (2*acos(q0))*180 / PI;
    float x = quat[1]; float y = quat[2]; float z = quat[3];
    rotation = makeVector4<float>(x,y,z, theta);
  }

  void set_frame(int f) {
    frame = f;
  }

  void display() {
    std::cout << "FRAME:\n";
    std::cout << frame << std::endl;
    std::cout << "ROTATION:\n";
    rotation.display();
    std::cout << "QUAT\n";
    quat.display();
    std::cout << "TRANSLATION:\n";
    translation.display();
    std::cout << "SCALE:\n";
    scale.display();
  }

};


#endif // __KEYFRAME_BLOCK_H_GUARD__
