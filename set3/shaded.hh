#ifndef __SHADED_H_GUARD__
#define __SHADED_H_GUARD__

#include <memory>
#include "blocks/scene_block.hh"

std::unique_ptr<SceneBlock> parse_test(std::istream &datafile);

#endif // __SHADED_H_GUARD__
