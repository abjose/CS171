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
  
  //std::cout << xmin << xmax << ymin << ymax << xres << yres << std::endl;

  //xmin = 0; xmax = 5;
  //ymin = 0; ymax = 5;
  //xres = 50; yres = 50;


  Canvas c;//(xmin, ymin, xmax, ymax, xres, yres);
  //c.clear(0);

  //std::cout << "Running test\n";
  //c.scale_draw_line(.2, .4, 1.3, 0.3);
  //c.draw_line(0, 0, 10, 10);

  c = parse_test(std::cin, xmin, ymin, xmax, ymax, xres, yres);

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

