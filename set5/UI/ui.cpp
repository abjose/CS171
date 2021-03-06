
#include "ui.hh"


void UI::motionFunction(const int x, const int y) {
  if (dragging) {
    s->set_ctrl_pt(drag_pt, getWorldX(x),-getWorldY(y));
  }

  glutPostRedisplay();
}

void UI::mouseFunction(int button, int state, int x, int y) {
  // disable interface if not showing points
  if(!s->show_pts) return;

  switch(button) {
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN) {
      lclick = true;

      // should see if user clicked on a control pt
      // if so, find it's index, and "pick it up"
      drag_pt  = s->get_ctrl_pt(getWorldX(x), -getWorldY(y));
      dragging = true;
    }
    else if(state == GLUT_UP) {
      lclick = false;
      dragging = false;
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if(state == GLUT_DOWN) {
      //rclick = true;
      if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	s->remove_ctrl_pt(getWorldX(x), -getWorldY(y));
      else
	s->insert_knot(getWorldX(x), -getWorldY(y));
    }
    else if(state == GLUT_UP) {
      //rclick = false;
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if(state == GLUT_DOWN) {
    int i = s->get_ctrl_pt(x,y);
    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
      s->p[i][2] -= 2; // decrease weight if shift
    else
      s->p[i][2] += 2; // else increase weight
    }
    break;
  default:
    break;
  }

  glutPostRedisplay();
}

float UI::getWorldX(int x) {
  return (xmin + ((float) x/xdim) * (xmax-xmin));
}
float UI::getWorldY(int y) {
  return (ymin + ((float) y/ydim) * (ymax-ymin));
}


void UI::keyFunction(GLubyte key, GLint x, GLint y) {
  switch (key) {
  case 'd':
    s->dot_mode = !s->dot_mode;
    break;
  case 't':
    s->toggle_ctrl_pts();
    break;
  case 27:
  case 'q':
  case 'Q':
    // escape or q or Q
    delete s;  // not sure where else to delete...
    exit(0);
    break;
  default:
    break;
  }

  glutPostRedisplay();
}
