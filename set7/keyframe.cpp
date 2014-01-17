
#include <iostream>
#include "keyframe.hh"
#include "framer.hh"
#include "UI/ui.hh"

static UI *ui;
static Framer f;

// The current window size.
int windowWidth = 800, windowHeight = 600;

/** GLUT callback functions **/

/*
 * This function gets called every time the window needs to be updated
 * i.e. after being hidden by another window and brought back into view,
 * or when the window's been resized.
 * You should never call this directly, but use glutPostRedisply() to tell
 * GLUT to do it instead.
 */
void redraw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  // APPLY UI TRANSFORMS
  //ui->applyViewingTransformation();
  //glTranslatef(ui->final_tx, -1*ui->final_ty, ui->final_tz);
  //glRotatef(ui->final_rd, ui->final_rx,ui->final_ry,0);
  
  auto T = f.get_next_frame();
  glRotatef(T->rotation[3], T->rotation[0], T->rotation[1], T->rotation[2]);
  glTranslatef(T->translation[0], T->translation[1], T->translation[2]);
  glScalef(T->scale[0], T->scale[1], T->scale[2]);

  makeIbar();

  glPopMatrix();
  glutSwapBuffers();
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

void makeIbar() {
  float r = 0.1;

  GLUquadricObj *lbot;
  GLUquadricObj *rbot;
  GLUquadricObj *mid;
  GLUquadricObj *ltop;
  GLUquadricObj *rtop;
  
  // bottom-left cylinder
  glPushMatrix();
  lbot = gluNewQuadric();
  glColor3f(1.0, 0.0, 0.0);
  glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, -1.0f, 0.0f);
  gluCylinder(lbot,r,r,1.0f,32,32);
  glPopMatrix();

  // bottom-right cylinder
  glPushMatrix();
  rbot = gluNewQuadric();
  glColor3f(0.0, 1.0, 0.0);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, -1.0f, 0.0f);
  gluCylinder(rbot,r,r,1.0f,32,32);
  glPopMatrix();

  // middle cylinder
  glPushMatrix();
  mid  = gluNewQuadric();
  glColor3f(1.0, 0.0, 1.0);
  glTranslatef(0.0f, 1.0f, 0.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

  gluCylinder(mid ,r,r,2.0f,32,32);
  glPopMatrix();

  // top-left cylinder
  glPushMatrix();
  ltop = gluNewQuadric();
  glColor3f(0.0, 0.0, 1.0);
  glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 1.0f, 0.0f);
  gluCylinder(ltop,r,r,1.0f,32,32);
  glPopMatrix();

  // top-right cylinder
  glPushMatrix();
  rtop = gluNewQuadric();
  glColor3f(0.0, 1.0, 1.0);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 1.0f, 0.0f);
  gluCylinder(rtop,r,r,1.0f,32,32);
  glPopMatrix();
}

/**
 * Set up OpenGL state.  This does everything so when we draw we only need to
 * actually draw the sphere, and OpenGL remembers all of our other settings.
 */
void initGL()
{
  glShadeModel(GL_SMOOTH); // gouraud
    
  // Enable back-face culling:
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Enable depth-buffer test.
  glEnable(GL_DEPTH_TEST);
    
  // Set up projection and modelview matrices ("camera" settings) 
  // Look up these functions to see what they're doing.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-0.5, 0.5, -0.5, 0.5, 1, 100);
  glFrustum(-1., 1., -1., 1., 1, 100);

  // from now on, everything should be in modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //gluLookAt(0, 0, 10, 0, 0, 0, 1, 0, 0);
  gluLookAt(0, 0, 20, -5, 0, 0, 1, 0, 0);
}

//--------------------------------------------------------------------------
// Handles reshaping of the program window
//--------------------------------------------------------------------------
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
// Initializes the UI
//--------------------------------------------------------------------------
void initUI()
{
  ui = new UI();
  resize(windowWidth, windowHeight);
  //checkGLErrors("End of uiInit");
}

/**
 * Main entrance point.
 * Sets up some stuff then passes control to glutMainLoop() which never
 * returns.
 */
int main(int argc, char* argv[])
{
  // parse stuff
  int num_frames;
  auto keyframes = parse(std::cin, num_frames);
  std::cout << "Got total frames by ref: " << num_frames << std::endl;

  f = Framer(keyframes, num_frames);
  f.display();

  // OpenGL will take out any arguments intended for its use here.
  // Useful ones are -display and -gldebug.
  glutInit(&argc, argv);
  
  // Get a double-buffered, depth-buffer-enabled window, with an
  // alpha channel.
  // These options aren't really necessary but are here for examples.
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(300, 100);

  glutCreateWindow("CS171 HW7");
    
  initGL();
  initUI();

  // set up GLUT callbacks.
  glutDisplayFunc(redraw);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyfunc);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  // From here on, GLUT has control,
  glutMainLoop();

  // so we should never get to this point.
  return 1;
}
