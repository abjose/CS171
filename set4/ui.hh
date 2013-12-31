#ifndef __UI_H_GUARD__
#define __UI_H_GUARD__

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include "matrix/transform.hh"

class UI {
private:
  // UI state stuff
  // mouse position
  int mx = 0;
  int my = 0;

  // TRANSLATION
  // state variables - set to 0 if off, 1 if on
  int is_translate = 0;
  int is_zoom = 0;
  // constants for translate and zoom normals
  float TC = 0.001;
  float ZC = 0.001;
  // x,y at which translation was started
  int init_tx = 0;
  int init_ty = 0;
  // y at which zoom was started
  int init_zy = 0;
  // amount to translate in PIXELS...worry about overflow?
  int translate_x = 0;
  int translate_y = 0;
  int translate_z = 0;

  // ROTATION


  // transform functions
  void startZoom();
  void endZoom();
  void startTranslation();
  void endTranslation();


public:
  float final_x;
  float final_y;
  float final_z;

  UI() {};
  ~UI() {};

  void motionFunction(const int x, const int y);
  void mouseFunction(int button, int state, int x, int y);
  void keyFunction(GLubyte key, GLint x, GLint y);
  void applyViewingTransformation();

};



#endif // __UI_H_GUARD__
