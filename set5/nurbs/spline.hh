#ifndef __SPLINE_H_GUARD__
#define __SPLINE_H_GUARD__

#include <vector>
#include <unordered_map>

#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

class Spline {
private:
  // control point - stores x,y,w
  typdef Matrix<float,3,1> CtrlPt;
  // order
  int k;
  // number of segments in the spline vector
  int res;
  // knot vector - should be nondecreasing, size n+k+1 where n = |p|
  std::vector<float> t;
  // control point vector
  std::vector<CtrlPt> p;
  // N_t ???
  // vector for holding points to draw...should make an array for OpenGL
  std::vector<Matrix<float,2,1> > spline;


public:
  Spline(int degree, int resolution) {
    k   = degree+1;
    res = resolution;
    if (degree == 3) {
      // if degree == 3, assume should populate for demo
      // populate control point vector with initial values
      float w = 1;
      p.push_back(makeVector3<float>(-1, 1,w));
      p.push_back(makeVector3<float>(-1,-1,w));
      p.push_back(makeVector3<float>( 1,-1,w));
      p.push_back(makeVector3<float>( 1, 1,w));
      // populate knot vector
      t = std::vector<float> {0,0,0,0,1,1,1,1};
    }
  }
  ~Spline() {}



  // will probably need some function for getting U from (x,y)


};



#endif // __SPLINE_H_GUARD__
