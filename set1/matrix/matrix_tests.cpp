#include <iostream>

#include "matrix.hh"
#include "transform.hh"


int main(int argc, char *argv[]) {

  Matrix<float,4,4> m1, m2, m3;
  Matrix<float,4,1> vec1, vec2;



  std::cout << "Running Matrix equality test\n";
  m1.clear(0); m2.clear(0); m3.clear(0);
  m1(1,1) = 1.3;  m2(1,1) = 1.3; m3(1,1) = 1.3;
  m1(1,3) = 1.9;  m2(1,3) = 1.9; m3(1,3) = 1.9;
  m1(2,1) = 2.3;  m2(2,1) = 2.3; m3(2,1) = 2.3;
  m1(2,3) = 4.2;  m2(2,3) = 4.2; m3(2,3) = 4.2;
  m1(0,1) = 1.4;  m2(0,1) = 1.4; m3(0,1) = 1.4;
  m1(0,3) = 1.3;  m2(0,3) = 1.3; m3(0,3) = 1.4;
  m1.display();
  m2.display();
  std::cout << "Equal?: " << (m1 == m2) << std::endl;
  m1.display();
  m3.display();
  std::cout << "Not Equal?: " << !(m1 == m3) << std::endl;
  if (m1 == m2 && !(m1 == m3))
    std::cout << "Test passed!\n";
  else
    std::cout << "Test failed :(\n";



  std::cout << "\nRunning Matrix*Matrix test\n";
  m2.make_identity();
  m1.display();
  m2.display();
  std::cout << "Multiplying...\nResult:\n";
  m3 = m1 * m2;
  m3.display();
  if (m3 == m1)
    std::cout << "Test passed!\n";
  else
    std::cout << "Test failed :(\n";



  std::cout << "\nRunning Matrix*Vector test\n";
  m1.make_identity();
  vec1 = makeVector4<float>(1.1,2.2,3.3,4.4);
  m1.display();
  vec1.display();
  std::cout << "Multiplying...\nResult:\n";
  vec2 = m1 * vec1;
  vec2.display();
  if (vec1 == vec2)
    std::cout << "Test passed!\n";
  else
    std::cout << "Test failed :(\n";



  std::cout << "\nRunning Matrix*Scalar test\n";
  m1.display();
  m2.clear(0);
  m2(0,0) = 3; m2(1,1) = 3; m2(2,2) = 3; m2(3,3) = 3;
  std::cout << "Multiplying by 3...\nResult:\n";
  m1 = m1 * 3;
  m1.display();
  if (m1 == m2)
    std::cout << "Test passed!\n";
  else
    std::cout << "Test failed :(\n";



  std::cout << "\nRunning Dot product test\n";
  vec1 = makeVector4<float>(1,2,3,4);
  vec2 = makeVector4<float>(2,4,6,8);
  vec1.display();
  vec2.display();
  std::cout << "Multiplying...\nResult: " << (vec1.dot(vec2)) << std::endl;
  if (vec1.dot(vec2) == (1*2 + 2*4 + 3*6 + 4*8))
    std::cout << "Test passed!\n";
  else
    std::cout << "Test failed :(\n";
  


  std::cout << "\nRunning Transpose test\n";
  m1.clear(0);
  m2.clear(0);
  m1(0,3) = 1;  m2(0,3) = -1;
  m1(3,0) = -1; m2(3,0) = 1;
  m1.display();
  m1 = m1.transpose();
  m1.display();
  if (m1 == m2)
    std::cout << "Test passed!\n";
  else
    std::cout << "Test failed :(\n";



  std::cout << "\nRunning Normalization test\n";
  vec1 = makeVector4<float>(1,2,3,4);
  vec1.display();
  std::cout << "Normalizing...\nResult:\n";
  vec1.normalize();
  vec1.display();
  std::cout << "Magnitude: " << vec1.magnitude() << std::endl;
  if (abs(vec1.magnitude()-1) < 0.0001)
    std::cout << "Test passed!\n";
  else
    std::cout << "Test failed :(\n";



  std::cout << "\nRunning Matrix*Matrix Inverse test\n";
  m1.clear(0); m2.clear(0); m3.clear(0);
  m1(0,0) = 4; m1(0,1) = 1; m1(0,2) = 9; m1(0,3) = 4;
  m1(1,0) = 1; m1(1,1) = 2; m1(1,2) = 4; m1(1,3) = 3;
  m1(2,0) = 8; m1(2,1) = 3; m1(2,2) = 2; m1(2,3) = 7;
  m1(3,0) = 6; m1(3,1) = 6; m1(3,2) = 6; m1(3,3) = 1;
  m1.display();
  std::cout << "Multiplying by inverse...\nResult:\n";
  m2 = m1;
  m1.inverse();
  m3 = m2 * m1;
  m3.display();


  return 0;
}
