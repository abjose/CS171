
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
    case GLUT_LEFT_BUTTON:
    // rotate on left
      if(state == GLUT_DOWN) {
	startRotation();
      }
      else if(state == GLUT_UP) {
	endRotation();
      }
      break;
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

  glutPostRedisplay();
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
  // set initial mouse location for translation
  init_tx = mx; init_ty = my;
}
void UI::endTranslation() {
  is_translate = 0;
  // add old translation to 'sum of translations' variables
  translate_x += mx - init_tx; 
  translate_y += my - init_ty;
}

// ZOOM
void UI::startZoom() {
  is_zoom = 1;
  // set initial mouse location for zoom
  init_zy = my;
}
void UI::endZoom() {
  is_zoom = 0;
  translate_z += my - init_zy;
}

// ROTATION
void UI::startRotation() {
  is_rotate = 1;
  // set initial mouse location for rotation
  init_rx = mx; init_ry = my;
}
void UI::endRotation() {
  is_rotate = 0;
  // add on old rotations
  rotate_x += mx - init_rx; 
  rotate_y += my - init_ry;
}

// TODO: get stuff from camera so can put center of rotation 3 units
//       in front of camera
// I guess just need to 1. figure out where that point is, 2. translate the
// world there, then 3. apply rotation?


void UI::applyViewingTransformation() {
  //glLoadIdentity();

  // apply UI transformations
  // SHOULD USE NORMALS!!!

  // translation
  final_tx = (translate_x + (mx-init_tx)*is_translate)*TC;
  final_ty = (translate_y + (my-init_ty)*is_translate)*TC;
  final_tz = (translate_z + (my-init_zy)*is_zoom)*ZC;
  std::cout << final_tx << ", " << final_ty << ", " << final_tz << "\n";

  // rotation
  // know perpendicular is 
  // get movement vector

  // NOTE THAT THIS IS ALL WRONG / COPIED
  // TODO: make perpendicular!!
  final_rx = (rotate_x + ((float) mx-init_rx)*is_rotate);//*RC;
  final_ry = (rotate_y + ((float) my-init_ry)*is_rotate);//*RC;
  //final_rz = (rotate_z + (my-init_rz)*is_rotate)*RC;
  //auto r_mv = makeVector2<float>(final_rx, final_ry);
  auto r_mv = makeVector2<float>(final_rx, final_ry);
  auto r_mag = r_mv.magnitude();
  r_mv = r_mv.normalize();
  final_rd = r_mag*RC;
  // move perpendicular
  //final_rx = r_mv[0];
  //final_ry = r_mv[1];
  final_rx = -r_mv[1];
  final_ry = r_mv[0];



  std::cout << final_rd << ", "
	    << final_rx << ", " << final_ry << ", " << final_rz << "\n";
}
