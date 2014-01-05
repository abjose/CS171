
#include <iostream>
#include "oglRenderer.hh"
//#include "uistate.h"
#include "UI/ui.hh"

static std::shared_ptr<SceneBlock> scene;
static UI *ui;

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

  // APPLY UI TRANSFORMS
  ui->applyViewingTransformation();
  glTranslatef(ui->final_tx, -1*ui->final_ty, ui->final_tz);
  glRotatef(ui->final_rd, ui->final_rx,ui->final_ry,0);

  // DO OBJECT STUFF
  for (auto& sep : scene->sep_list) {
    // INITIALIZE MATERIAL FOR THIS SEPARATOR
    GLfloat emit[] = {0.0, 0.0, 0.0, 1.0}; // wat
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

    // RENDER
    sep->render();  // TODO: change name to populate_blah or something
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, sep->vertices);
    glNormalPointer(GL_FLOAT, 0, sep->normals);
    glDrawArrays(GL_TRIANGLES, 0, sep->n_verts);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    

    // POP TRANSFORMATIONS
    glPopMatrix();
  }
  
  glPopMatrix();
  glutSwapBuffers();
}

void redraw_lines() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glTranslatef(0.01,0,0);

  glPushMatrix();

  // apply camera transform?
  //ui->ApplyViewingTransformation();
  ui->applyViewingTransformation();
  glTranslatef(ui->final_tx, -1*ui->final_ty, ui->final_tz);
  //glRotatef(ui->final_rd, ui->final_rx, -1*ui->final_ry, ui->final_rz);
  //glTranslatef(3,0,0);
  glRotatef(ui->final_rd, ui->final_rx,ui->final_ry,0);
  //glTranslatef(-3,0,0);

  //glLoadIdentity();  // might...not need?
  //glPushMatrix();

  // TODO: so...put this stuff here explicitly?
  //GLfloat emit[], amb[], diff[], spec[], shiny;
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

    // RENDER 
    glColor3ub( 255,255,255 );
    glLineWidth(1.0);
    
    glBegin(GL_LINE_STRIP);
    for (auto& pt : sep->vertex_list)
      glVertex3f(pt[0], pt[1], pt[2]);
    glEnd();
    
    // POP TRANSFORMATIONS
    glPopMatrix();
  }
  
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
  if (mode != 0 && !scene->is_lines)
    initLights();
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
  // read cmd line args
  int mode;
  mode = std::stoi(argv[1]);
  windowWidth = std::stoi(argv[2]); windowHeight = std::stoi(argv[3]);

  // From old code
  scene = parse(std::cin);
  
  // OpenGL will take out any arguments intended for its use here.
  // Useful ones are -display and -gldebug.
  glutInit(&argc, argv);
  
  // Get a double-buffered, depth-buffer-enabled window, with an
  // alpha channel.
  // These options aren't really necessary but are here for examples.
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(300, 100);

  glutCreateWindow("CS171 HW4");
    
  initGL(mode);
  initUI();

  // set up GLUT callbacks.
  if (scene->is_lines)
    // A bit limiting - modes mutually exclusive. But OK for now, hopefully.
    glutDisplayFunc(redraw_lines);
  else
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
