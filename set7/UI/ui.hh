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

 

  // TRANSLATION
  // state variables - set to 0 if off, 1 if on
  int is_translate = 0;
  int is_zoom = 0;
  int is_rotate = 0;
  // constants for translate and zoom normals
  float TC = 0.005;
  float ZC = 0.005;
  float RC = 0.05;
  // x,y at which translation was started
  int init_tx = 0;
  int init_ty = 0;
  // y at which zoom was started
  int init_zy = 0;
  // x,y at which rotation was started
  int init_rx = 0;
  int init_ry = 0;
  // amount to translate in PIXELS...worry about overflow?
  int translate_x = 0;
  int translate_y = 0;
  int translate_z = 0;
  // rotation stuff
  int rotate_d = 0;
  int rotate_x = 0;
  int rotate_y = 0;
  int rotate_z = 0;

  // ROTATION


  // transform functions
  void startZoom();
  void endZoom();
  void startTranslation();
  void endTranslation();
  void startRotation();
  void endRotation();


public:
  float final_tx;
  float final_ty;
  float final_tz;

  float final_rd;
  float final_rx;
  float final_ry;
  float final_rz;

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
  void mouseFunction(int button, int state, int x, int y);
  void keyFunction(GLubyte key, GLint x, GLint y);
  void applyViewingTransformation();
  void specialFunction(int key, int x, int y);

};



#endif // __UI_H_GUARD__
