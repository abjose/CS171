#ifndef __LIGHT_BLOCK_H_GUARD__
#define __LIGHT_BLOCK_H_GUARD__

#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

class LightBlock{
public:
  //private:
  Matrix<float,3,1> location; // x y z
  Matrix<float,3,1> color;    // r g b

  //public:
  LightBlock() {
    location = makeVector3<float>(0,0,1);
    color = makeVector3<float>(1,1,1);
  }

  void set_location(float x, float y, float z) {
    location = makeVector3<float>(x,y,z);
  }
  void set_color(float r, float g, float b) {
    color = makeVector3<float>(r,g,b);
  }

  void display() {
    std::cout << "SHOWING LIGHT'S LOCATION\n";
    location.display();
    std::cout << "SHOWING LIGHT'S COLOR\n";
    color.display();
  }
};


#endif // __LIGHT_BLOCK_H_GUARD__
