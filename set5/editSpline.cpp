
#include <iostream>
#include "editSpline.hh"
#include "nurbs/spline.hh"
#include "UI/ui.hh"


//static UIState *ui;
static UI *ui;
// spline
static Spline *s;

// The current window size.
int windowWidth = 600, windowHeight = 600;

/** Utility functions **/

/**
 * Set up OpenGL state.  This does everything so when we draw we only need to
 * actually draw the sphere, and OpenGL remembers all of our other settings.
 */
void initGL()
{
  // enable anti-aliasing
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


/** GLUT callback functions **/

/**
 * GLUT calls this function when the window is resized.
 */
void resize(GLint w, GLint h)
{
  if (h == 0) h = 1;

  // ensure that we are always square (even if whole window not used)
  if (w > h) w = h;
  else       h = w;

  // Reset the current viewport and perspective transformation
  glViewport(0, 0, w, h);

  // Tell GLUT to call redraw()
  glutPostRedisplay();
}

//--------------------------------------------------------------------------
// Handles motion of the mouse when a button is being held
//--------------------------------------------------------------------------
void motion(const int x, const int y)
{
  // Just pass it on to the ui controller.
  ui->motionFunction(x, y);
}

//--------------------------------------------------------------------------
// Handles mouse clicks and releases
//--------------------------------------------------------------------------
void mouse(const int button, const int state, const int x, const int y)
{
  // Just pass it on to the ui controller.
  ui->mouseFunction(button, state, x, y);
}

//--------------------------------------------------------------------------
// Handles keybaord input
//--------------------------------------------------------------------------
void keyboard(GLubyte key, GLint x, GLint y)
{
  // Just pass it on to the ui controller.
  ui->keyFunction(key,x,y);
}


void display() {
  glEnable(GL_LINE_SMOOTH);

  // recalc stuff
  s->make_spline();

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2, 2, -2, 2, -1, 1);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  // draw control point connections
  if (s->show_pts) {
    glColor3ub( 127, 127, 127 );
    glLineWidth(1.0);

    glLineStipple(3,0xAAAA);
    glEnable(GL_LINE_STIPPLE); 
    glBegin(GL_LINE_STRIP);
    for (auto& pt : s->p)
      glVertex2f(pt[0], pt[1]);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
  }

  glColor3ub( 255, 255, 255 );
  glLineWidth(3.0);

  // draw spline
   glBegin(GL_LINE_STRIP);
  for (auto& pt : s->spline)
    glVertex2f(pt[0], pt[1]);
  glEnd();


  if (s->show_pts) {
    glColor3ub( 127, 127, 127 );
    // draw control points
    glPointSize(10.0);
    glBegin(GL_POINTS);
    for (auto& p : s->p)
      glVertex2f(p[0], p[1]);
    glEnd();
  }

  glutSwapBuffers();
}

/**
 * Main entrance point.
 * Sets up some stuff then passes control to glutMainLoop() which never
 * returns.
 */
int main(int argc, char* argv[])
{
  s = new Spline(3);
  ui = new UI(-2,2, -2,2, 600,600, s); // make sure to change args...

  if (argc == 2) {
    s->set_filename(argv[1]);
  }

  // TODO: ui doesn't work if resize screen - don't hard-pass dims!
  // TODO: sometimes removing first and last points seems to break it

  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize( 600, 600 );
  glutCreateWindow("CS171 HW4");

  initGL();

  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  return 1;
}





