#ifndef __LOADTEXTURE_H__
#define __LOADTEXTURE_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <png.h>
#include <cstdio>
#include <string>

#define TEXTURE_LOAD_ERROR 0


GLuint loadTexture(const std::string filename, int &width, int &height);


#endif // __LOADTEXTURE_H__
