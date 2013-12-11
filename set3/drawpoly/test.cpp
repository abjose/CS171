#include <stdint.h>
#include <iostream>
#include "image.h"
#include "draw.h"

using namespace std;

PPM ppm(400, 400);

#define X 0
#define Y 1
#define Z 2
#define R 3
#define G 4
#define B 5
void drawFunc(vertex_t vert)
{
  pcolor pc = RGB((int)(vert[R] * 255.0), (int)(vert[G] * 255.0), (int)(vert[B] * 255.0));
  ppm.setPixel(int(vert[X]), int(vert[Y]), vert[Z], pc);
}

void clipAndDrawPolygon(polygon_t poly, func_t drawFunc)
{
   polygon_t newpoly;
   newpoly = clipPolygon(poly);
   index_t newlen = newpoly.size();

   // quickly change x-->screenx, y-->screeny, leave everything else alone
   for (int i = 0; i < newlen; i++)
     {
       newpoly[i][0] = getX(newpoly[i][0]);
       newpoly[i][1] = getY(newpoly[i][1]);
     }
   drawPolygon(newpoly, drawFunc);
}

int main(int argc, char *argv[]) {
   polygon_t poly, poly2;
   vertex_t vert;
   vert.clear();
   vert.push_back(-1.2);
   vert.push_back(0);
   vert.push_back(0);
   vert.push_back(1.0);
   vert.push_back(0);
   vert.push_back(0);
   poly.push_back(vert);
   vert.clear();
   vert.push_back(0);
   vert.push_back(1.2);
   vert.push_back(0);
   vert.push_back(0.0);
   vert.push_back(0.9);
   vert.push_back(0.0);
   poly.push_back(vert);
   vert.clear();
   vert.push_back(1.2);
   vert.push_back(-0.5);
   vert.push_back(0);
   vert.push_back(0.0);
   vert.push_back(0.0);
   vert.push_back(0.8);
   poly.push_back(vert);

   vert.clear();
   vert.push_back(0);
   vert.push_back(0.7);
   vert.push_back(1.0);
   vert.push_back(0);
   vert.push_back(1);
   vert.push_back(1);
   poly2.push_back(vert);
   vert.clear();
   vert.push_back(-0.7);
   vert.push_back(-0.7);
   vert.push_back(-1.0);
   vert.push_back(1);
   vert.push_back(1);
   vert.push_back(0);
   poly2.push_back(vert);
   vert.clear();
   vert.push_back(0.7);
   vert.push_back(-0.5);
   vert.push_back(-1.0);
   vert.push_back(1);
   vert.push_back(0);
   vert.push_back(1);
   poly2.push_back(vert);

   initDraw(-1,1,-1,1,400,400);

   clipAndDrawPolygon(poly, drawFunc);
   clipAndDrawPolygon(poly2, drawFunc);

   ppm.write(cout);

   return 0;
}
