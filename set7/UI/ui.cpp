
#include <iostream>

#include "ui.hh"


void UI::motionFunction(const int x, const int y) {
  mx = x; 
  my = y;

  // Tell GLUT to call redraw()
  glutPostRedisplay();
}



void UI::keyFunction(GLubyte key, GLint x, GLint y) {
  switch (key) {
  case 'p':
  case 'P':
    // play
    f->playing = true;
    std::cout << "Playing.\n";
    break;
  case 's':
  case 'S':
    // stop
    f->playing = false;
    std::cout << "Stopped.\n";
    break;
  case 'f':
  case 'F':
    // forward
    f->curr_frame += 1;
    std::cout << "Moving foward a frame.\n";
    break;
  case 'r':
  case 'R':
    // reverse
    f->curr_frame -= 1;
    std::cout << "Moving backward a frame.\n";
    break;
  case 'l':
  case 'L':
    // toggle loop mode
    f->looping = !f->looping;
    if (f->looping)
      std::cout << "Looping on.\n";
    else
      std::cout << "Looping off.\n";
    break;
  case 'j':
  case 'J':
    // jump to frame
    int frm;
    std::cout << "Jump to frame: ";
    std::cin >> frm;
    f->curr_frame = frm;
    break;
  case '0':
    // jump to start
    std::cout << "Resetting to 0th frame.\n";
    f->curr_frame = 0;
    break;
  case 27:
  case 'q':
  case 'Q':
    // escape or q or Q
    exit(0);
    break;
  }
   

  glutPostRedisplay();
}

void UI::specialFunction(int key, int x, int y) {
  switch(key) {
  case GLUT_KEY_UP:
    cam_z += 1;
    break;
  case GLUT_KEY_DOWN:
    cam_z -= 1;
    break;
  case GLUT_KEY_LEFT:
    cam_y += 1;
    break;
  case GLUT_KEY_RIGHT:
    cam_y -= 1;
    break;

  }

  glutPostRedisplay();
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

void UI::applyViewingTransformation() {
  // translation
  final_tx = (translate_x + (mx-init_tx)*is_translate)*TC;
  final_ty = (translate_y + (my-init_ty)*is_translate)*TC;
  final_tz = (translate_z + (my-init_zy)*is_zoom)*ZC;

  // rotation
  final_rx = (rotate_x + ((float) mx-init_rx)*is_rotate);//*RC;
  final_ry = (rotate_y + ((float) my-init_ry)*is_rotate);//*RC;
  auto r_mv = makeVector2<float>(final_rx, final_ry);
  auto r_mag = r_mv.magnitude();
  r_mv = r_mv.normalize();
  final_rd = r_mag*RC;
  // move perpendicular
  final_rx = -r_mv[1];
  final_ry = r_mv[0];
}
