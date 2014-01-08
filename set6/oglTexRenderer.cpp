
#include <iostream>
#include <string>
#include "oglTexRenderer.hh"
//#include "uistate.h"
#include "UI/ui.hh"
//#include "libpng/ogl_readpng.h"
#include "libpng/loadtexture.hh"


static std::shared_ptr<SceneBlock> scene;
static UI *ui;

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

    /*
    // INITIALIZE MATERIAL FOR THIS SEPARATOR
    GLfloat emit[] = {0.0, 0.0, 0.0, 0.0}; // wat
    GLfloat amb[] = {sep->material->ambient[0],
		     sep->material->ambient[1],
		     sep->material->ambient[2]};
    GLfloat diff[] = {sep->material->diffuse[0],
		      sep->material->diffuse[1],
		      sep->material->diffuse[2]};
    GLfloat spec[] = {sep->material->specular[0],
		      sep->material->specular[1],
		      sep->material->specular[2]};
    GLfloat shiny = sep->material->shininess;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_EMISSION, emit);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shiny);
    */

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
    //glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, sep->vertices);
    //glNormalPointer(GL_FLOAT, 0, sep->normals);
    glTexCoordPointer(2, GL_FLOAT, 0, sep->texcoords);
    glDrawArrays(GL_TRIANGLES, 0, sep->n_verts);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    

    // POP TRANSFORMATIONS
    glPopMatrix();
  }

  glPopMatrix();
  glutSwapBuffers();
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
 * Sets up an OpenGL light.  This only needs to be called once
 * and the light will be used during all renders.
 */
void initLights() {
  // pretty sure can just add to GL_LIGHT0, but probably shouldn't depend on...
  int light_consts[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4,
			GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

  for (int i=0; i<scene->lights.size(); i++) {  
    auto l   = scene->lights[i];
    int  l_c = light_consts[i];

    GLfloat amb[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat spec[] = {l->color[0], l->color[1], l->color[2], 1.0f};
    GLfloat diff[] = {l->color[0], l->color[1], l->color[2], 1.0f};
    GLfloat lightpos[] = { l->location[0],l->location[1],l->location[2], 1.0f };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    glLightfv(l_c, GL_AMBIENT, amb);
    glLightfv(l_c, GL_DIFFUSE, diff);
    glLightfv(l_c, GL_SPECULAR, spec);
    glLightfv(l_c, GL_POSITION, lightpos);
    glEnable(l_c);
  }

  // Turn on lighting.  You can turn it off with a similar call to
  // glDisable().
  glEnable(GL_LIGHTING);
}

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

  // set light parameters if not doing wireframe
  //if (mode != 0 && !scene->is_lines)
  //  initLights();
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


// //COPIED
// void init(void) {
//   glClearColor(0.0, 0.0, 0.0, 0.0);
//   glEnable(GL_DEPTH_TEST);
  
//   int width, height;
//   std::string filename("data/cubetex.png");
//   GLuint t = loadTexture(filename, width, height);
//   if (t == 0) std::cout << "UH OH SOMETHING SCREWED UP\n";
//   std::cout << "Image loaded: " << width << " " << height << std::endl;
  
//   //glShadeModel(GL_FLAT);
// }

//COPIED
void display(void) {
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -3.6);
  glRotatef(rotateX, 0,1,0);
  glRotatef(rotateY, 1,0,0);
 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-2.0, -1.0, 0.0);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-2.0, 1.0, 0.0);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.0, 1.0, 0.0);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.0, -1.0, 0.0);
 
  glEnd();
  glutSwapBuffers();
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
  //if (scene->is_lines)
    // A bit limiting - modes mutually exclusive. But OK for now, hopefully.
    //glutDisplayFunc(redraw_lines);
  //else
  glutDisplayFunc(redraw);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyfunc);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  // From here on, GLUT has control,
  glutMainLoop();

  // so we should never get to this point.
  return 1;

  


  /*
  // COPIED!!

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("PNG texture");
  glutMotionFunc(mouseMotion);
  glutPassiveMotionFunc(mousePassive);
  init();
  glutReshapeFunc(myReshape);
  glutDisplayFunc(display);
  //std::cout << "Use mouse drag to rotate." << std::endl;
  glutMainLoop();
  return 0;
  */


}
