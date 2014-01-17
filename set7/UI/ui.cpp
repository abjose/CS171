
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

