
#include <fstream>
#include <vector>
#include "parse_transform.hh"

using namespace std;

int main(int argc, char *argv[]) {

  // parse cin
  std::cout << "PARSING!\n";
  Matrix<float,4,4> T = parse_test(cin);

  // set up a test vector
  Matrix<float,4,1> v;
  v = makeVector4<float>(1,2,3,4);

  // show transformation
  std::cout << "\nVECTOR BEFORE TRANSFORMS:\n";
  v.display();
  std::cout << "\nFINAL TRANSFORMATION:\n";
  T.display();
  std::cout << "\nVECTOR AFTER TRANSFORMS:\n";
  (T*v).display();
  
  return 0;
}
