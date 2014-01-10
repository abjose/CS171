
#include <iostream>
#include <string>
#include "oglTexRenderer.hh"
//#include "uistate.h"
#include "UI/ui.hh"
//#include "libpng/ogl_readpng.h"
#include "libpng/loadtexture.hh"


static std::shared_ptr<SceneBlock> scene;
static UI *ui;
static float water_time;

// The current window size.
int windowWidth = 800, windowHeight = 600;

//GLubyte *textureImage;

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
  ui->applyViewingTransformation();
  glTranslatef(ui->final_tx, -1*ui->final_ty, ui->final_tz);
  glRotatef(ui->final_rd, ui->final_rx,ui->final_ry,0);

  // DO OBJECT STUFF
  for (auto& sep : scene->sep_list) {
    // PUSH TRANSFORMATIONS
    glPushMatrix();
    
    // TRANSFORM TO WORLD SPACE
    for (auto& t : sep->transforms) {
      glTranslatef(t->translation[0],
		   t->translation[1],
		   t->translation[2]);
      glRotatef(t->rotation[3],
		t->rotation[0],
		t->rotation[1],
		t->rotation[2]);
      glScalef(t->scale[0],
	       t->scale[1],
	       t->scale[2]);
    }

    // load texture? should avoid doing every time...
    int twidth, tsize;
    GLuint texres = loadTexture(std::string("data/") + sep->tex_filename, 
				twidth,tsize);

    // RENDER
    sep->render();  // TODO: change name to populate_blah or something
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, sep->vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, sep->texcoords);
    glDrawArrays(GL_TRIANGLES, 0, sep->n_verts);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // POP TRANSFORMATIONS
    glPopMatrix();
  }

  glPopMatrix();

  redraw_water();

  glutSwapBuffers();
}

void redraw_water() {
  // test code for water ripples

  int w,h;
  // note: intead of loading each time, just switch back and forth
  // using the texture integer lols
  loadTexture(std::string("data/sky.png"), w,h);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);

  // apply UI transformation
  glPushMatrix();
  ui->applyViewingTransformation();
  glTranslatef(0, -2.5, -5);
  glTranslatef(ui->final_tx, -1*ui->final_ty, ui->final_tz);
  glRotatef(ui->final_rd, ui->final_rx,ui->final_ry,0);
 
  Matrix<float,3,1> N1,N2;
  Matrix<float,3,1> N3,N4;

  float prev_y = ymin;

  // loop to make plane out of triangle strips
  // this is very inefficient, don't use immediate mode after debug...
  for(float y=ymin; y<ymax; y+=dy) {
    glBegin(GL_TRIANGLE_STRIP);
    for(float x=xmin; x<xmax; x+=dx) {
      // calculate ripply normals
      N1[0] = 2.*x*sin(x*x + y*y - water_time);
      N1[1] = 2.*y*sin(x*x + y*y - water_time);
      N1[2] = 1;
      N1 = N1.normalize();
      N2[0] = 2.*x*sin(x*x + prev_y*prev_y - water_time);
      N2[1] = 2.*prev_y*sin(x*x + prev_y*prev_y - water_time);
      N2[2] = 1;
      N2 = N2.normalize();

      // set vertices and texcoords
      glNormal3f(N1[0], N1[1], N1[2]); glVertex3f(y, 0, x);
      glNormal3f(N2[0], N2[1], N2[2]); glVertex3f(prev_y, 0, x);
    }
    glEnd();
    prev_y = y; // could just subtract dy...
  }
  
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);

  glPopMatrix();
  glutSwapBuffers();
  
}

void water_clock() {
  water_time += 0.05;
  glutPostRedisplay();
}


// removed redraw_lines

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
 * Set up OpenGL state.  This does everything so when we draw we only need to
 * actually draw the sphere, and OpenGL remembers all of our other settings.
 */
void initGL(int mode)
{
  switch(mode) {
  case 0: // wireframe
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    break;
  case 1: // flat
    glShadeModel(GL_FLAT);   // flat 
    break;
  case 2: // gouraud
    glShadeModel(GL_SMOOTH); // gouraud
    break;
  default:
    break;
  }  
    
  // Enable back-face culling:
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Enable depth-buffer test.
  glEnable(GL_DEPTH_TEST);

  // The following two lines enable semi transparent
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // enable textures
  glEnable(GL_TEXTURE_2D);
    
  // Set up projection and modelview matrices ("camera" settings) 
  // Look up these functions to see what they're doing.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(scene->camera->left,
	    scene->camera->right,
	    scene->camera->bottom,
	    scene->camera->top,
	    scene->camera->near,
	    scene->camera->far);

  // from now on, everything should be in modelview
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();

  // camera rotate -theta
  glRotatef(-1*scene->camera->rotation[3],
	    scene->camera->rotation[0],
	    scene->camera->rotation[1],
	    scene->camera->rotation[2]);
  // camera -translate
  glTranslatef(-1*scene->camera->position[0], 
 	         -1*scene->camera->position[1], 
  	         -1*scene->camera->position[2]);
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
}



void mousePassive(int x, int y){
  mouseX = x;
  mouseY = y;
}

void mouseMotion(int x, int y){
  const float SPEED = 2;
    
  rotateX += (mouseX-x)/SPEED;
  rotateY += (mouseY-y)/SPEED;
  mousePassive(x, y);
  glutPostRedisplay();
}

void myReshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0 * (GLfloat) w / (GLfloat) h, 1.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
}



/**
 * Main entrance point.
 * Sets up some stuff then passes control to glutMainLoop() which never
 * returns.
 */
int main(int argc, char* argv[])
{
  // initialize time for water
  water_time = 0;

  // init array for texture normals
  dx = 0.1; xmin = -5; xmax = 5;
  dy = 0.1; ymin = -5; ymax = 5;

  // read cmd line args
  int mode;
  mode = std::stoi(argv[1]);
  windowWidth = std::stoi(argv[2]); windowHeight = std::stoi(argv[3]);

  // From old code
  scene = parse(std::cin);
  //scene->display();
  
  // OpenGL will take out any arguments intended for its use here.
  // Useful ones are -display and -gldebug.
  glutInit(&argc, argv);

  // Get a double-buffered, depth-buffer-enabled window, with an
  // alpha channel.
  // These options aren't really necessary but are here for examples.
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(300, 100);

  glutCreateWindow("CS171 HW6");
    
  initGL(mode);
  initUI();

  // set up GLUT callbacks.
  glutDisplayFunc(redraw);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyfunc);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutIdleFunc(water_clock);
  glutReshapeFunc(myReshape);
 
  // From here on, GLUT has control,
  glutMainLoop();

  // we should never get to this point.
  return 1;
}
