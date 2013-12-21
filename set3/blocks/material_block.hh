#ifndef __MATERIAL_BLOCK_H_GUARD__
#define __MATERIAL_BLOCK_H_GUARD__

#include "../matrix/matrix.hh"

class MaterialBlock{
public:
  //private:
  Matrix<float,3,1> ambient;
  Matrix<float,3,1> diffuse;
  Matrix<float,3,1> specular;

  //public:
  float shininess;

  void set_ambient(float r, float g, float b) {
    ambient[0] = r; ambient[1] = g; ambient[2] = b;
  }
  void set_diffuse(float r, float g, float b) {
    diffuse[0] = r; diffuse[1] = g; diffuse[2] = b;
  }
  void set_specular(float r, float g, float b) {
    specular[0] = r; specular[1] = g; specular[2] = b;
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
