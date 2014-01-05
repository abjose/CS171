
#include <iostream>
#include "editSpline.hh"
#include "nurbs/spline.hh"
#include "UI/ui.hh"


//static UIState *ui;
static UI *ui;
// spline
static Spline *s;

// The current window size.
int windowWidth = 800, windowHeight = 600;

/** GLUT callback functions **/

/**
 * GLUT calls this function when the window is resized.
 * All we do here is change the OpenGL viewport so it will always draw in the
 * largest square that can fit in our window..
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


/*
 * GLUT calls this function when any key is pressed while our window has
 * focus.  Here, we just quit if any appropriate key is pressed.  You can
 * do a lot more cool stuff with this here.
 */
void keyfunc(GLubyte key, GLint x, GLint y)
{
  switch (key) {
  // escape or q or Q
  case 27:
  case 'q':
  case 'Q':
    exit(0);
    break;
  }
   
}

/** Utility functions **/

/**
 * Sets up an OpenGL light.  This only needs to be called once
 * and the light will be used during all renders.
 */

/**
 * Set up OpenGL state.  This does everything so when we draw we only need to
 * actually draw the sphere, and OpenGL remembers all of our other settings.
 */
void initGL()
{
  glShadeModel(GL_SMOOTH); // gouraud
  //glShadeModel(GL_FLAT);   // flat
  //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe
  // TODO: SHOULD TURN OFF LIGHTING AND STUFF FOR THIS ONE!!
  
    
  // Enable back-face culling:
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_LINE_SMOOTH);

  // Enable depth-buffer test.
  glEnable(GL_DEPTH_TEST);
    
  // Set up projection and modelview matrices ("camera" settings) 
  // Look up these functions to see what they're doing.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // glFrustum(scene->camera->left,
  // 	    scene->camera->right,
  // 	    scene->camera->bottom,
  // 	    scene->camera->top,
  // 	    scene->camera->near,
  // 	    scene->camera->far);

  // NOTE: THE REST OF EVERYTHING WILL BE IN MODELVIEW
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();

  // camera rotate -theta
  // glRotatef(-1*scene->camera->rotation[3],
  // 	    scene->camera->rotation[0],
  // 	    scene->camera->rotation[1],
  // 	    scene->camera->rotation[2]);
  // camera -translate
  // glTranslatef(-1*scene->camera->position[0], 
  // 	         -1*scene->camera->position[1], 
  // 	         -1*scene->camera->position[2]);

  // set light parameters
  //initLights();

  // set material parameters
  //initMaterial();
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

void display()
{
  // recalc stuff
  s->make_spline();

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2, 2, -2, 2, -1, 1);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glEnable(GL_POINT_SMOOTH);
  //glEnable(GL_BLEND);

  glColor3ub( 127, 127, 127 );
  glLineWidth(1.0);

  // draw control point connections
  glLineStipple(3,0xAAAA);
  glEnable(GL_LINE_STIPPLE); 
  glBegin(GL_LINE_STRIP);
  for (auto& pt : s->p)
    glVertex2f(pt[0], pt[1]);
  glEnd();
  glDisable(GL_LINE_STIPPLE);

  // draw control points
  glPointSize(10.0);
  glBegin(GL_POINTS);
  for (auto& p : s->p)
    glVertex2f(p[0], p[1]);
  glEnd();

  glColor3ub( 255, 255, 255 );
  glLineWidth(3.0);

  // draw spline
   glBegin(GL_LINE_STRIP);
  for (auto& pt : s->spline)
    glVertex2f(pt[0], pt[1]);
  glEnd();

  glutSwapBuffers();
}



/**
 * Main entrance point.
 * Sets up some stuff then passes control to glutMainLoop() which never
 * returns.
 */
int main(int argc, char* argv[])
{
  /*
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   
  initGL();
  */


  // just test out spline stuff for now
  s = new Spline(3, 1000);
  ui = new UI(-2,2, -2,2, 600,600, s); // make sure to change args...

  //s->insert_knot(0,0);

  s->make_spline();

  s->display();

  // TODO: ui doesn't work if resize screen - don't hard-pass dims!

  //std::cout << std::endl;
  //for (auto& p : s->spline)
  //  std::cout << p[0] << ", " << p[1] << std::endl;


  // TODO: clean this shit up
  // also, get rid of CANVAS!! don't really need it here

  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize( 600, 600 );
  glutCreateWindow("CS171 HW4");
  glutDisplayFunc( display );
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();

  return 1;
}





