#include <iostream>
#include <string>
#include "shaded.hh"
#include "matrix/canvas.hh"
#include "raster/raster.h"

int main(int argc, char *argv[]) {
  // read cmd line args
  int mode, xres, yres;
  mode = std::stoi(argv[1]);
  xres = std::stoi(argv[2]); yres = std::stoi(argv[3]);
  float xMin = -1; float yMin = -1;
  float xMax =  1; float yMax =  1;
  
  // call initDraw to initialize raster code for later
  initDraw(xMin, xMax, yMin, yMax, xres, yres);

  // make a new canvas and scene for the parser
  auto c = std::make_shared<Canvas>(Canvas(xMin, yMin, xMax, yMax, xres, yres));
  std::shared_ptr<SceneBlock> scene;
  scene = parse(std::cin);

  // add eyelight if desired
  if (argc == 5 && std::string(argv[4])=="-eyelight") {
    std::cerr << "Eyelight added\n";
    auto l = std::shared_ptr<LightBlock>(new LightBlock());
    auto pos = scene->camera->position;
    l->set_location(pos[0],pos[1],pos[2]);
    l->set_color(0,1,0);
    scene->lights.push_back(l);
  }

  // uncomment to show contents of Scene after parsing
  // (should probably comment the ppm stuff below)
  //scene->display(); 

  // render to the canvas
  scene->render(c, mode);

  // spit the PPM to stdout
  std::cout << c->to_ppm() << std::endl;

  std::cerr << "Done.\n";
  
  return 0;
}

