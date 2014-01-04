#include <iostream>
#include "wireframe.hh"
#include "matrix/canvas.hh"

int main(int argc, char *argv[]) {
  // read cmd line args
  int xres, yres;
  xres = std::stoi(argv[1]); yres = std::stoi(argv[2]); 

  // make a new canvas and scene for the parser
  Canvas c(-1, -1, 1, 1, xres, yres);
  std::unique_ptr<SceneBlock> scene;
  scene = parse_test(std::cin);

  // uncomment to display contents of scene
  // (should also comment PPM stuff below)
  //scene->display();

  // render to the canvas
  scene->render(c);

  // spit the PPM to std output
  std::cout << c.to_ppm() << std::endl;
  
  return 0;
}

