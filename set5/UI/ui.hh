#ifndef __UI_H_GUARD__
#define __UI_H_GUARD__

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include "../matrix/transform.hh"
#include "../nurbs/spline.hh"

class UI {
private:

  // window -> world cord stuff
  float xmin, xmax, ymin, ymax;
  int xdim, ydim;

  // spline
  Spline* s;

  // click state
  bool lclick = false;
  bool rclick = false;
  // drag state
  bool dragging = false;
  int drag_pt = -1;

  float getWorldX(int x);
  float getWorldY(int y);

public:

  UI(float xMin, float xMax, float yMin, float yMax, int xDim, int yDim,
     Spline* S) {
    xmin = xMin; xmax = xMax;
    ymin = yMin; ymax = yMax;
    xdim = xDim; ydim = yDim;
    s = S;
  };
  ~UI() {};

  void motionFunction(const int x, const int y);
  void mouseFunction(int button, int state, int x, int y);
  void keyFunction(GLubyte key, GLint x, GLint y);
};



#endif // __UI_H_GUARD__
