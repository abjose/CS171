
#include <fstream>
#include <vector>
#include "polyline.hh"

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Running test\n";
  
  std::string test_str = "scaleFactor 100 100 100";
  std::ifstream file(argv[1]);
  
  //std::vector<Matrix<float,4,4>* > ms = parse_test(cin);
  std::vector<Matrix<float,4,4> > ms = parse_test(file);

  Matrix<float,4,1> v;
  v.clear(0);
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;

  // TODO: think should change back to using cin rather than a file - that
  //       way you can just pipe in the file contents or whatever

  v.display();
  v.apply_transform_stack(ms);
  
  return 0;
}
