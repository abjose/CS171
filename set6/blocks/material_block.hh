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
    ambient = makeVector3<float>(r,g,b);
  }
  void set_diffuse(float r, float g, float b) {
    diffuse = makeVector3<float>(r,g,b);
  }
  void set_specular(float r, float g, float b) {
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
