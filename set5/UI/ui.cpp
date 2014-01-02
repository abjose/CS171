
#include "ui.hh"




void UI::motionFunction(const int x, const int y) {
  mx = x; 
  my = y;

  //std::cout << x << ", " << y << std::endl;

  // SURE THIS SHOULD BE HERE??
  // Tell GLUT to call redraw()
  glutPostRedisplay();
}



void UI::mouseFunction(int button, int state, int x, int y) {
  mx = x;
  my = y;

  switch(button) {
    //case GLUT_LEFT_BUTTON:
    // rotate on left
    //if(state == GLUT_DOWN) ;   //OnLButtonDown(x, y);
    //else if(state == GLUT_UP) ;//OnLButtonUp(x, y);
    //break;
  case GLUT_MIDDLE_BUTTON:
    if(state == GLUT_DOWN) {
      if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	startZoom();
      else
	startTranslation();
    }
    else if(state == GLUT_UP) {
      if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	// this might not work as desired...
	// probably should add 'on-the-fly' so that shift being released doesn't
	// reset things...
	endZoom();
      else
	endTranslation();
    }
    break;
    // just put another switch state in based on glutGetModifiers()?
    /*
  case GLUT_RIGHT_BUTTON:
    if(state == GLUT_DOWN) {
      OnRButtonDown(x, y);
      mx = x;
      my = y;
    }
    else if(state == GLUT_UP) OnRButtonUp(x, y);
    break;
    */
  default:
    break;
  }

  //glutPostRedisplay();
}


void UI::keyFunction(GLubyte key, GLint x, GLint y) {
  switch (key) {
  case 27:
  case 'q':
  case 'Q':
    // escape or q or Q
    exit(0);
    break;
  default:
    break;
  }
   
}

// TRANSLATION
void UI::startTranslation() {
  is_translate = 1;
  // set initial mouse location for zoom
  init_tx = mx; init_ty = my;
}
void UI::endTranslation() {
  is_translate = 0;
  // add old translation to 'sum of translations' variables
  translate_x += mx-init_tx; 
  translate_y += my-init_ty;
}

// ZOOM
void UI::startZoom() {
  is_zoom = 1;
  // set initial mouse location for zoom
  init_zy = my;
}
void UI::endZoom() {
  is_zoom = 0;
  translate_z += my-init_zy;
}

// ROTATION
// get other ones working first...?


void UI::applyViewingTransformation() {
  //glLoadIdentity();

  // WAIT, if just push and pop each time (before calling this) then don't
  // need to do camera stuff over and over again...
  
  /*
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


  // apply UI transformations
  // SHOULD USE NORMALS!!!
  // translation
  final_x = (translate_x + (mx-init_tx)*is_translate)*TC;
  final_y = (translate_y + (my-init_ty)*is_translate)*TC;
  final_z = (translate_z + (my-init_zy)*is_zoom)*ZC;
  //std::cout << final_x << ", " << final_y << ", " << final_z << "\n";
  std::cout << final_x << ", " << final_y << ", " << final_z << "\n";
  //glTranslatef(translate_x*TC, -1*translate_y*TC, translate_z*ZC);
  //std::cout << glGetError() << std::endl;

  //glTranslatef(0,0,0);
  // rotation
  //glRotatef(degrees, x, y, z);
}
