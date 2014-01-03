#ifndef __MATERIAL_BLOCK_H_GUARD__
#define __MATERIAL_BLOCK_H_GUARD__

#include <algorithm>

#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

class MaterialBlock{
public:
  //private:
  Matrix<float,3,1> ambient;
  Matrix<float,3,1> diffuse;
  Matrix<float,3,1> specular;

  //public:
  float shininess;

  MaterialBlock() {
    set_ambient(.2,.2,.2);
    set_diffuse(.8,.8,.8);
    set_specular(0,0,0);
    shininess = 0.2;
  }

  void set_ambient(float r, float g, float b) {
    // clamp values
    r = std::max(0.f, std::min(1.f, r));
    g = std::max(0.f, std::min(1.f, g));
    b = std::max(0.f, std::min(1.f, b));
    ambient = makeVector3<float>(r,g,b);
  }
  void set_diffuse(float r, float g, float b) {
    // clamp values
    r = std::max(0.f, std::min(1.f, r));
    g = std::max(0.f, std::min(1.f, g));
    b = std::max(0.f, std::min(1.f, b));
    diffuse = makeVector3<float>(r,g,b);
  }
  void set_specular(float r, float g, float b) {
    // clamp values
    r = std::max(0.f, std::min(1.f, r));
    g = std::max(0.f, std::min(1.f, g));
    b = std::max(0.f, std::min(1.f, b));
    specular = makeVector3<float>(r,g,b);
  }

  void display() {
    std::cout << "SHOWING MATERIAL'S AMBIENT COLOR:\n";
    ambient.display();
    std::cout << "SHOWING MATERIAL'S DIFFUSE COLOR:\n";
    diffuse.display();
    std::cout << "SHOWING MATERIAL'S SPECULAR COLOR:\n";
    specular.display();
    std::cout << "SHOWING MATERIAL'S SHININESS:\n";
    std::cout << shininess << std::endl;
  }
};


#endif // __MATERIAL_BLOCK_H_GUARD__
