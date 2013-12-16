#ifndef __DRAW2D_H_GUARD__
#define __DRAW2D_H_GUARD__

#include <memory>
#include "blocks/scene_block.hh"

std::unique_ptr<SceneBlock> parse_test(std::istream &datafile);

#endif // __DRAW2D_H_GUARD__
