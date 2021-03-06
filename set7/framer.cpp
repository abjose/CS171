
#include <iostream>
#include <algorithm>
#include "framer.hh"

template <typename T>
T Framer::basic_interpolate(float u, T k0, T k1) {
  return k0*(2*pow(u,3)-3*u*u+1) + k1*(3*u*u-2*pow(u,3));
}

template <typename T>
T Framer::interpolate(float u, T k0, T k1, T k2, T k3) {
  // interpolate between k1 and k2 using catmull-rom
  float dx = 1.;  // right?
  T k1_p = ((k1-k0)/dx)*0.5 + ((k2-k1)/dx)*0.5;
  T k2_p = ((k2-k1)/dx)*0.5 + ((k3-k2)/dx)*0.5;
  return k1*(2*pow(u,3)-3*u*u+1) + k2*(3*u*u-2*pow(u,3)) + 
    k1_p*(pow(u,3)-2*u*u+u) + k2_p*(pow(u,3)-u*u);
}

void Framer::quatify() {
  // take keyframe vector and convert rotations to quaternions
  for (auto &key : keyframes)
    key->find_quat();
}

void Framer::framify() {
  // copy the last frame with the desired final number of frames if necessary
  // should probably do something like this for first set as well...
  auto back = keyframes.back();
  auto last = std::shared_ptr<KeyframeBlock>(new KeyframeBlock());
  last->scale = back->scale;
  last->quat = back->quat;
  last->translation = back->translation;
  last->frame = num_frames;
  keyframes.push_back(last);

  // take quatified keyframe vector and populate the frames vector appropriately
  for (int keyframe_idx=0; keyframe_idx<keyframes.size()-1; keyframe_idx++) {
    // just doing simple interpolation for now
    auto f0 = keyframes[std::max<int>(0,keyframe_idx-1)];
    auto f1 = keyframes[keyframe_idx];
    auto f2 = keyframes[keyframe_idx+1]; // make sure to stop one before end
    auto f3 = keyframes[std::min<int>(keyframes.size()-1, keyframe_idx+2)];

    // figure out how many frames to make for current keyframe pair, and du
    int num_frames = f2->frame - f1->frame; // go s.t. [f1,f2)
    float du = 1./num_frames;
 
    // loop to make that many frames, pushing onto frames vector as you go
    float u = 0;
    for(int i=0; i<num_frames; i++) {
      auto frame = std::shared_ptr<KeyframeBlock>(new KeyframeBlock());
      frame->scale = interpolate<Matrix<float,3,1> >
	(u, f0->scale, f1->scale, f2->scale, f3->scale);
      frame->quat = interpolate<Matrix<float,4,1> >
	(u, f0->quat, f1->quat, f2->quat, f3->quat);
      frame->translation = interpolate<Matrix<float,3,1> >
	(u, f0->translation, f1->translation, f2->translation, f3->translation);
      frames.push_back(frame);

      u += du;
    }
  }
}

void Framer::dequatify() {
  // take generated frames and convert quaternions back to  old rotations
  for (auto &f : frames)
    f->find_rot();
}

std::shared_ptr<KeyframeBlock> Framer::get_next_frame() {
  // just use an iterator instead?
  auto frame = frames[std::max<int>(std::min<int>(frames.size()-1, curr_frame),
				    0)];
  if (playing)
    curr_frame += 1;
  if (looping && curr_frame >= frames.size())
    curr_frame = 0;
  return frame;
}

void Framer::display() {
  std::cout << "Displaying keyframes:\n";
  for (auto &key : keyframes) {
    key->display();
  }
  std::cout << "Displaying frames:\n";
  for (auto &f: frames) {
    f->display();
  }
}
