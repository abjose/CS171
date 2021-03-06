#ifndef __FRAMER_H_GUARD__
#define __FRAMER_H_GUARD__

#include <memory>
#include <vector>

#include "blocks/keyframe_block.hh"

class Framer {
private:
  std::vector<std::shared_ptr<KeyframeBlock> > keyframes;
  int num_frames;

  void quatify();
  void framify();
  void dequatify();
  template <typename T>
  T basic_interpolate(float u, T k0, T k1);
  template <typename T>
  T interpolate(float u, T k0, T k1, T k2, T k3);

public:
  std::vector<std::shared_ptr<KeyframeBlock> > frames;
  int curr_frame;
  bool looping = true;
  bool playing = true;

  // default constructor...don't use...
  Framer() {}
  Framer(std::vector<std::shared_ptr<KeyframeBlock> > k, int f) {
    curr_frame = 0;
    keyframes = k;
    num_frames = f;
    quatify();
    framify();
    dequatify();
  }

  std::shared_ptr<KeyframeBlock> get_next_frame();
  void display();

};










#endif // __FRAMER_H_GUARD__
