#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "../matrix/matrix.hh"
#include "../matrix/transform.hh"
#include "../matrix/canvas.hh"
#include "draw2d.hh"

int main(int argc, char *argv[]) {
  float xmin, ymin, xmax, ymax;
  int xres, yres;
  
  xmin = std::stof(argv[1]); xmax = std::stof(argv[2]); 
  ymin = std::stof(argv[3]); ymax = std::stof(argv[4]); 
  xres = std::stoi(argv[5]); yres = std::stoi(argv[6]); 

  // store the result of parsing into a Canvas object for drawing
  Canvas c;
  c = parse_test(std::cin, xmin, ymin, xmax, ymax, xres, yres);
  
  // write the PPM data to stdout
  std::cout << c.to_ppm() << std::endl;
  

  // draw2d [xmin] [xmax] [ymin] [ymax] [xRes] [yRes]
  // NOTE that the lines can go off the screen, outside the image space.
  // MAKE SURE TO TEST DRAWING LINES THAT GO OFF THE CANVAS
  // If doesn't already work, could just add an in-bounds test to the drawing
  // code.
  // Also, verify negative xmins/xmaxs work...
  // Also, clean up remnants of old code.

   return 0;
}

