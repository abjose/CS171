

#include <iostream>
//using namespace std;
//#include <stdio.h>

#include "matrix.hh"
#include "transform.hh"




int main() {
  Matrix<int,3,3> m1;
  Matrix<int,3,5> m2;
  Matrix<double,4,4> m3;
  Matrix<double,4,4> test;
  Matrix<double,4,1> diag;
  diag.clear(2);

  Matrix<double,4,4> m = rotation_matrix<double>(1,2,1,1.5);
  m.display();

  m1.clear(1);
  m2.clear(2);
  m3.clear(1);
  m3.set_diagonal(diag);
  test = m3;
  m1(1,1) = 3;
  m1(2,1) = 5;
  m1.display();
  m2.display();
  m2 = m1*m2;
  m2.display();
  m1 = m1.transpose();
  m1.display();
  m1.make_identity();
  m1.display();
  m3.display();
  std::cout << "dot!: " << diag.dot(diag) << std::endl;
  bool inverted = m3.invert();
  if(inverted) {
    m3.display();
    test = test*m3;
    test.display();
  }
  else
    std::cout << "DIDN'T INVERT" << std::endl;



  // TEST TRANSFORMATIONS!!!
}

