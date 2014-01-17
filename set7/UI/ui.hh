#ifndef __UI_H_GUARD__
#define __UI_H_GUARD__

#include <memory>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include "../framer.hh"
#include "../matrix/transform.hh"

class UI {
private:
  // UI state stuff
  // mouse position
  int mx = 0;
  int my = 0;

  // global
  std::shared_ptr<Framer> f;

public:
  // cam stuff
  float cam_x = -10;
  float cam_y = -10; 
  float cam_z = -10;

  UI() {}
  UI(std::shared_ptr<Framer> fp) {
    f = fp;
  };
  ~UI() {};

  void motionFunction(const int x, const int y);
  void keyFunction(GLubyte key, GLint x, GLint y);
  void specialFunction(int key, int x, int y);
};



#endif // __UI_H_GUARD__
