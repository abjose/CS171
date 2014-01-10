#ifndef __SHADED_H_GUARD__
#define __SHADED_H_GUARD__

#include <memory>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include "blocks/scene_block.hh"

GLubyte *textureImage;

float rotateX = 0;
float rotateY = 0;

int mouseX;
int mouseY;


/** PROTOTYPES **/
void initLights();
void initMaterial();
void redraw();
void initGL();
void resize(GLint w, GLint h);
void keyfunc(GLubyte key, GLint x, GLint y);

void redraw_water();

// from old code
std::shared_ptr<SceneBlock> parse(std::istream &datafile);


#endif // __SHADED_H_GUARD__
