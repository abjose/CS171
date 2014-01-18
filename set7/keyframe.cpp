
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include "keyframe.hh"
#include "framer.hh"
#include "UI/ui.hh"

static UI *ui;
static std::shared_ptr<Framer> f;

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

  // move camera as desired and look at origin
  glLoadIdentity();
  gluLookAt(ui->cam_x, ui->cam_y, ui->cam_z, -5, 0, 0, 1, 0, 0);
  
  // get current frame's transformations
  auto T = f->get_next_frame();
  glScalef(T->scale[0], T->scale[1], T->scale[2]);
  glRotatef(T->rotation[3], T->rotation[0], T->rotation[1], T->rotation[2]);
  glTranslatef(T->translation[0], T->translation[1], T->translation[2]);

  // draw I-bar
  makeIbar();
  glutSwapBuffers();
}

/** Utility functions **/

void makeIbar() {
  // draw the I-bar
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
  glFrustum(-1., 1., -1., 1., 1, 100);

  // from now on, everything should be in modelview
  glMatrixMode(GL_MODELVIEW);
}

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

void motion(const int x, const int y)
{
  // Just pass it on to the ui controller.
  ui->motionFunction(x, y);
}

void keyfunc(const GLubyte key, GLint x, GLint y) {
  // Just pass it on to the ui controller.
  ui->keyFunction(key, x, y);
}

void specialfunc(const int key, const int x, const int y)
{
  // Just pass it on to the ui controller.
  ui->specialFunction(key, x, y);
}

void initUI()
{
  ui = new UI(f);
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
  // open file
  std::string filename(argv[1]);
  std::ifstream file(filename);
  if (!file.is_open())
    exit(1);

  // parse stuff
  int num_frames;
  //auto keyframes = parse(std::cin, num_frames);  
  auto keyframes = parse(file, num_frames);  
  f = std::shared_ptr<Framer>(new Framer(keyframes, num_frames));
  //f->display();

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
  glutIdleFunc(redraw);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyfunc);
  glutSpecialFunc(specialfunc);
  glutMotionFunc(motion);

  // From here on, GLUT has control,
  glutMainLoop();

  // so we should never get to this point.
  return 1;
}
