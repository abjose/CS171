#ifndef __SPLINE_H_GUARD__
#define __SPLINE_H_GUARD__

#include <vector>
#include <unordered_map>

#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"

class Spline {
private:
  // control point - stores x,y,w
  typedef Matrix<float,3,1> CtrlPt;
  // order
  int k;
  // number of segments in the spline vector
  int res;
  // knot vector - should be nondecreasing, size n+k+1 where n = |p|
  std::vector<float> t;
  
  // N_t --- unordered map for doing dynamic programming later

  // math-y things
  float  B_i(int i, float u);
  float  N(int i, int k_, float u);
  //float N_dynamic(i, k_, u);
  CtrlPt Q(float u);
  float  a(int i, int j, float t_new);

  // knot manipulation
  void  insert_knot(float t_new);
  float get_knot_from_pt(float x, float y);
  int   find_knot_index(float u);

public:
  // vector for holding points to draw
  std::vector<Matrix<float,2,1> > spline;
  // control point vector
  std::vector<CtrlPt> p;

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
      // make initial spline
      make_spline();
    }
  }
  ~Spline() {}

  // public knot manipulation
  void insert_knot(float x, float y);

  // control point manipulation
  int get_ctrl_pt(float x, float y);
  void set_ctrl_pt(int index, float x, float y);

  // spline-generation
  void make_spline();

  // output
  void display_knots();
  void display_pts();
  void display();
};



#endif // __SPLINE_H_GUARD__
