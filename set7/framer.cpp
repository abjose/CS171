
#include <iostream>
#include "framer.hh"


/*
Need...
- something to take keyframes, copy them if necessary for later passing on to
  interpolation thing (for the slopes), and convert rotations to quaternions
- something to take processed keyframes and interpolate, filling up a new
  vector with the actual frames
- Modify TransformBlock to having a quat?
- SHOULD WRITE ANOTHER THING TO GO BACK FROM QUAT TO ROTATION STUFF
 */

template <typename T>
T Framer::basic_interpolate(float u, T k0, T k1) {

  return k0*(2*pow(u,3)-3*u*u+1) + k1*(3*u*u-2*pow(u,3));
}

template <typename T>
T Framer::interpolate(float u, T k0, T k1, T k2, T k3) {
  // interpolate between k1 and k2 using catmull-rom
  float dx = 1.;  // what should this be??
  T k1_p = ((k1-k0)/dx)*0.5 + ((k2-k1)/dx)*0.5;
  T k2_p = ((k2-k1)/dx)*0.5 + ((k3-k2)/dx)*0.5;
  return k1*(2*pow(u,3)-3*u*u+1) + k2*(3*u*u-2*pow(u,3)) + 
    k1_p*(pow(u,3)-2*u*u+u) + k2_p*(pow(u,3)-u*u);
}

void Framer::quatify() {
  // take keyframe vector and convert rotations to quaternions
  // also copy first and last elements? (to enable better interpolation)
  for (auto &key : keyframes) {
    key->find_quat_rot();
  }
  // could, instead of copying first and last, just deal with that in the final
  // loop?
}

void Framer::framify() {
  // copy the last frame with the desired final number of frames if necessary
  auto back = keyframes.back();
  auto last = std::shared_ptr<KeyframeBlock>(new KeyframeBlock());
  last->scale = back->scale;
  last->quat_rot = back->quat_rot;
  last->translation = back->translation;
  last->frame = num_frames;
  keyframes.push_back(last);

  // take quatified keyframe vector and populate the frames vector appropriately
  for (int keyframe_idx=0; keyframe_idx<keyframes.size()-1; keyframe_idx++) {
    // just doing simple interpolation for now
    auto f1 = keyframes[keyframe_idx];
    auto f2 = keyframes[keyframe_idx+1]; // make sure to stop one before end
    
    // figure out how many frames to make for current keyframe pair, and du
    int num_frames = f2->frame - f1->frame; // go s.t. [f1,f2)
    std::cout << "f1 frame: " << f1->frame << std::endl;
    std::cout << "f2 frame: " << f2->frame << std::endl;
    std::cout << "Number of frames to make for this pair: " << num_frames << std::endl;
    float du = 1./num_frames;
    std::cout << "du: " << du << std::endl;


    // loop to make that many frames, pushing onto frames vector as you go
    float u = 0;
    for(int i=0; i<num_frames; i++) {
      auto frame = std::shared_ptr<KeyframeBlock>(new KeyframeBlock());
      frame->scale = basic_interpolate<Matrix<float,3,1> >
	(u, f1->scale, f2->scale);
      frame->quat_rot = basic_interpolate<glm::gtc::quaternion::quat>
	(u, f1->quat_rot, f2->quat_rot);
      frame->translation = basic_interpolate<Matrix<float,3,1> >
	(u, f1->translation, f2->translation);
      frames.push_back(frame);
      //frame->display();
      //std::cout << "\n\n";
      u += du;
    }
  }
}

void Framer::dequatify() {
  // take generated frames and convert quaternions back old rotations
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
