#include <iostream>
#include "shaded.hh"
#include "matrix/canvas.hh"
#include "raster/raster.h"

int main(int argc, char *argv[]) {
  // read cmd line args
  int mode, xres, yres;
  //xres = std::stoi(argv[1]); yres = std::stoi(argv[2]); 
  mode = 2;
  xres = 400; yres = 400;
  float xMin = -1; float yMin = -1;
  float xMax =  1; float yMax =  1;
  
  // TODO: convert things from using .display() to << notation
  // TODO: change canvas to take args in same order as initDraw?
  // TODO: convert Canvas back to non-template because always using rgb...

  // call initDraw for use of raster later
  initDraw(xMin, xMax, yMin, yMax, xres, yres);

  // make a new canvas and scene for the parser
  auto c = std::make_shared<Canvas>(Canvas(xMin, yMin, xMax, yMax, xres, yres));
  std::shared_ptr<SceneBlock> scene;
  scene = parse(std::cin);

  // render to the canvas
  //scene->display();
  scene->render(c, mode);

  // spit the ppm to std output
  std::cout << c->to_ppm() << std::endl;
  
  return 0;
}

