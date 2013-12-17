#ifndef __MATERIAL_BLOCK_H_GUARD__
#define __MATERIAL_BLOCK_H_GUARD__

#include "../matrix/matrix.hh"

class MaterialBlock{
private:
  Matrix<float,4,1> ambient;
  Matrix<float,4,1> diffuse;
  Matrix<float,4,1> specular;

public:
  // public because laziness
  float shininess;

  void set_ambient(float r, float g, float b) {
    ambient[0] = r; ambient[1] = g; ambient[2] = b; ambient[3] = 1;
  }
  void set_diffuse(float r, float g, float b) {
    diffuse[0] = r; diffuse[1] = g; diffuse[2] = b; diffuse[3] = 1;
  }
  void set_specular(float r, float g, float b) {
    specular[0] = r; specular[1] = g; specular[2] = b; specular[3] = 1;
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
