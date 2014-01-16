#ifndef __SHADED_H_GUARD__
#define __SHADED_H_GUARD__

#include <memory>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include "blocks/keyframe_block.hh"

/* 
 * A quadric can be a cylinder, disk, sphere, cone, etc.
 * We just reuse this memory space each time we draw any sort of quad.
 * GLU supplies this funcionality.
 */
GLUquadricObj* quad;

/** PROTOTYPES **/
void initLights();
void initMaterial();
void redraw();
void initGL();
void resize(GLint w, GLint h);
void keyfunc(GLubyte key, GLint x, GLint y);

// from old code
std::vector<std::shared_ptr<KeyframeBlock> >
parse(std::istream &datafile, int &frames);

#endif // __SHADED_H_GUARD__
