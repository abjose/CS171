
#include <iostream>
#include "shaded.hh"
#include "nurbs/spline.hh"
#include "UI/ui.hh"


//static UIState *ui;
static UI *ui;
// spline
static Spline *s;

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

  //glTranslatef(0.01,0,0);

  glPushMatrix();

  // apply camera transform?
  //ui->ApplyViewingTransformation();
  //ui->applyViewingTransformation();
  //glTranslatef(ui->final_x, -1*ui->final_y, ui->final_z);

  //glLoadIdentity();  // might...not need?
  //glPushMatrix();

  //scene->render();

  // TODO: put object transformations in here???
  // and just like...everything?!!!

  /*
  for each frame:
    // NOTE!!: these out push/pops should be moved when incorporating 
    //         user input!
    push

    // user mouse translate and rotation -- more details later
    
    for each object:
      push
      transform(s)
      //draw the polygons
      pop
    pop
  */
  
  glPopMatrix();

  glutSwapBuffers();
}

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
void initLights() {
  // TODO: Ask connor if these are right? especially spec vs. diff and shiny

  // pretty sure can just add to GL_LIGHT0, but probably shouldn't depend on...
  int light_consts[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4,
			GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

  // for (int i=0; i<scene->lights.size(); i++) {  
  //   auto l   = scene->lights[i];
  //   int  l_c = light_consts[i];

  //   GLfloat amb[] = { 0.0, 0.0, 0.0, 1.0 };
  //   GLfloat spec[] = {l->color[0], l->color[1], l->color[2], 1.0f};
  //   //GLfloat diff[] = {l->color[0], l->color[1], l->color[2], 1.0f};
  //   GLfloat diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  //   GLfloat lightpos[] = { l->location[0],l->location[1],l->location[2], 1.0f };
  //   //GLfloat shiny = 4.0f; 

  //   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
  //   glLightfv(l_c, GL_AMBIENT, amb);
  //   glLightfv(l_c, GL_DIFFUSE, diff);
  //   glLightfv(l_c, GL_SPECULAR, spec);
  //   glLightfv(l_c, GL_POSITION, lightpos);
  //   //glLightf(l_c, GL_SHININESS, shiny);
  //   glEnable(l_c);
  //}

  // Turn on lighting.  You can turn it off with a similar call to
  // glDisable().
  glEnable(GL_LIGHTING);
}

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
  initLights();

  // set material parameters
  //initMaterial();
}






//--------------------------------------------------------------------------
// Handles reshaping of the program window
//--------------------------------------------------------------------------
//void reshape(const int width, const int height)
/*
void resize(const int width, const int height)
{
    windowWidth = width;
    windowHeight = height;
    
    if( width <= 0 || height <= 0 ) return;
    
    ui->WindowX() = width;
    ui->WindowY() = height;
    
    ui->Aspect() = float( width ) / height;
    ui->SetupViewport();
    ui->SetupViewingFrustum();
}
*/

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
  /*
glFrustum(scene->camera->left,
	    scene->camera->right,
	    scene->camera->bottom,
	    scene->camera->top,
	    scene->camera->near,
	    scene->camera->far);

  // NOTE: THE REST OF EVERYTHING WILL BE IN MODELVIEW
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


*/

  //ui = new UI();
  //ui = new UIState;
  /*
  ui->Trans() = Vector3(scene->camera->position[0],
			scene->camera->position[1],
			scene->camera->position[2]);
  ui->Radius() = 2;
  ui->Near() = scene->camera->near;;
  ui->Far() = scene->camera->far;
  */
  //reshape(windowWidth, windowHeight);
  resize(windowWidth, windowHeight);
  //checkGLErrors("End of uiInit");
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

  glColor3ub( 255, 255, 255 );

  // draw spline
  glBegin(GL_LINE_STRIP);
  for (auto& pt : s->spline)
    glVertex2f(pt[0], pt[1]);
  glEnd();

  // draw control point connections
  glBegin(GL_LINE_STRIP);
  for (auto& pt : s->p)
    glVertex2f(pt[0], pt[1]);
  glEnd();

  // draw control points
  glPointSize(10.0);
  glEnable(GL_POINT_SMOOTH);
  //glEnable(GL_BLEND);
  glBegin(GL_POINTS);
  for (auto& p : s->p)
    glVertex2f(p[0], p[1]);
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

    UI TODO:
    - set up keyboard stuff to maintain key state in UI class
    - add stuff for properly translating camera, etc. to UI class
    - add stuff to update camera transformation
    - move transformation stuff to redraw (think already is)
    - make zoom (shift+drag middle mouse)
    - make translate (middle mouse button)
    - make rotation (don't do arcball? unless code easy to copy)
    -- yeahhh, maybe don't do arcball...especially with quaternions...

   */


  // REFERENCE CODE OUTLINE (for transformations)
  /*
  // perspective and camera are setup only once in the beginning of program
  PROJECTION
  Frustum
  
  MODELVIEW
  camera rotate -theta
  camera -translate
  
  for each frame:
    push

    // user mouse translate and rotation -- more details later
    
    for each object:
      push
      transform(s)
      //draw the polygons
      pop
    pop
  */

  // TODO: OH NO LOOKS LIKE ONE FACE ISN'T BEING RENDERED IN FOURCUBES


  /*
  // OpenGL will take out any arguments intended for its use here.
  // Useful ones are -display and -gldebug.
  glutInit(&argc, argv);
  
  // Get a double-buffered, depth-buffer-enabled window, with an
  // alpha channel.
  // These options aren't really necessary but are here for examples.
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(800, 800);
  glutInitWindowPosition(300, 100);

  glutCreateWindow("CS171 HW4");
    
  initGL();
  initUI();

  // set up GLUT callbacks.
  glutDisplayFunc(redraw);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyfunc);
  //glutIdleFunc(glutPostRedisplay);

  //glutDisplayFunc(display);
  //glutIdleFunc(glutPostRedisplay);
  //glutKeyboardFunc(keyboard);
  //glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  // From here on, GLUT has control,
  glutMainLoop();

  // so we should never get to this point.
  return 1;

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


  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize( 600, 600 );
  glutCreateWindow( "GLUT" );
  glutDisplayFunc( display );
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();

  return 1;
}





