#ifndef __OGL_READPNG_H
#define __OGL_READPNG_H
//#include <png.h>

//png_bytepp readpng(const char *filename, int* width, int* height);
bool loadPngImage(char *name, int &outWidth, int &outHeight, 
		  bool &outHasAlpha, GLubyte **outData);

#endif
