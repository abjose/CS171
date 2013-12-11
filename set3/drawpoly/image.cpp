/* Class for a 24-bit color canvas.  Outputs to an ostream in PPM
 * format.
 */
#include <ostream>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <cstring>
#include "image.h"

using namespace std;

#define FAKE_INFINITY 9.9e99

PPM::PPM(const int width, const int height)
   : width(width), height(height) {
   data = new pcolor[width*height];
   depth = new double[width*height];
   memset(data, 0, width*height*sizeof(pcolor));
   // Initialize the depth of each pixel to INFINITY.
   for(int i = 0; i < width*height; i++)
      depth[i] = FAKE_INFINITY;
}

PPM::~PPM() {
   delete[] data;
   delete[] depth;
}

pcolor &PPM::getPixel(int x, int y) {
   assert(x>=0);
   assert(y>=0);
   assert(x<width);
   assert(y<height);

   return data[y*width + x];
}

double &PPM::getDepth(int x, int y) {
   assert(x>=0);
   assert(y>=0);
   assert(x<width);
   assert(y<height);

   return depth[y*width + x];
}

static bool reported = false;
void PPM::setPixel(int x, int y, double z, pcolor c) {
   // Draw the pixel if it's in bounds and is above the current depth
   // at that position.
   if(0 <= x && x < width &&
      0 <= y && y < height && getDepth(x,y) > z) {
      getPixel(x,y) = c;
      getDepth(x,y) = z;
      if (!reported)
        {
          cerr << "Pixel drawn at " << x << ' ' << y << ' ' << z << endl;
          //cerr << "Pixel of color " << RED(c) << ' ' << GREEN(c) << ' ' << BLUE(c) << endl;
          reported = true;
        }
   }
   else if (!reported)
   {
      cerr << "Pixel out of bounds at " << x << ' ' << y << endl;
      reported = true;
   }
}

void PPM::write(ostream &os) {
   // PPM header
   os << "P3" << endl;
   os << width << ' ' << height << endl;
   os << 255 << endl;

   // Each pixel is a set of three integers in ASCII decimal format.
   for(int i = 0; i < width*height; i++) {
      pcolor &c = data[i];
      os << RED(c) << ' ' << GREEN(c) << ' ' << BLUE(c) << endl;
   }

   os.flush();
}
