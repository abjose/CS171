#ifndef __SHADED_H_GUARD__
#define __SHADED_H_GUARD__

#include <memory>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

/** PROTOTYPES **/
void initLights();
void initMaterial();
void redraw();
void initGL();
void resize(GLint w, GLint h);
void keyfunc(GLubyte key, GLint x, GLint y);


#endif // __SHADED_H_GUARD__
