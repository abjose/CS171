#ifndef __LIGHT_BLOCK_H_GUARD__
#define __LIGHT_BLOCK_H_GUARD__

#include "../matrix/matrix.hh"

class LightBlock{
private:
  Matrix<float,4,1> location; // x y z w
  Matrix<float,4,1> color;    // r g b ...a?

public:

  void set_location(float x, float y, float z) {
    location[0] = x;
    location[1] = y;
    location[2] = z;
    location[3] = 1;
  }
  void set_color(float r, float g, float b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = 1;
  }

  void display() {
    std::cout << "SHOWING LIGHT'S LOCATION\n";
    location.display();
    std::cout << "SHOWING LIGHT'S COLOR\n";
    color.display();
  }
};


#endif // __LIGHT_BLOCK_H_GUARD__
