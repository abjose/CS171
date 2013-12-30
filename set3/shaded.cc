#include <iostream>
#include "shaded.hh"
#include "matrix/canvas.hh"
#include "raster/raster.h"

int main(int argc, char *argv[]) {
  // read cmd line args
  int mode, xres, yres;
  //xres = std::stoi(argv[1]); yres = std::stoi(argv[2]); 
  mode = 1;
  xres = 400; yres = 400;
  float xMin = -1; float yMin = -1;
  float xMax =  1; float yMax =  1;
  
  // TODO: convert things from using .display() to << notation
  // TODO: change canvas to take args in same order as initDraw?
  // TODO: why do lights consistently seem to be on the wrong side?
  // TODO: why isn't backface culling not working? possibly putting vertices
  //       in wrong order somewhere?
  // TODO: Also possible clipping just isn't working...didn't write anything
  //       specifically for it...

  // TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
  // ALEX! When transforming surface normals, DON'T have W be 1 - have it be 0 
  // and then completely ignore it (i.e. don't normalize...)
  // http://stackoverflow.com/questions/10596548/correctly-transforming-a-surface-normal

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

