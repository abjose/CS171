#ifndef __LIGHTING_H_GUARD__
#define __LIGHTING_H_GUARD__

#include <vector>
#include <math.h>
#include <memory>
#include "raster/raster.h"
#include "blocks/light_block.hh"
#include "blocks/material_block.hh"
#include "blocks/transform_block.hh"
#include "blocks/camera_block.hh"
#include "matrix/matrix.hh"
#include "matrix/canvas.hh"
#include "matrix/transform.hh"

// TODO: move this to a sensible directory
// TODO: add z-buff stuff...presumable to draw_pixels funcs
// TODO: move some of these imports to the cc file

// this right?
// vector of float arrays for drawing pixels onto the canvas
static std::vector<float*> pixels_to_draw;


Matrix<float,3,1> light_func(Matrix<float,3,1> n, Matrix<float,3,1> v,
			     std::shared_ptr<MaterialBlock> material, 
			     //std::vector<LightBlock*> lights, 
			     LightBlock& light, 
			     Matrix<float,3,1> camerapos);

void draw_pixels_with_constant_color(Canvas<Matrix<float,3,1> > &c, 
				     Matrix<float,3,1> color);

void draw_pixels(Canvas<Matrix<float,3,1> >& c);

void draw_flat(int x, int y, float *data);
void flat_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		  Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		  Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		  std::shared_ptr<MaterialBlock> material,
		  //std::vector<LightBlock*> lights,
		  LightBlock& light,
		  CameraBlock& camera, 
		  std::shared_ptr<TransformBlock> transform,
		  Canvas<Matrix<float,3,1> >& c);

void draw_gourand(int x, int y, float *data);
void gourand_shading();

void draw_phong(int x, int y, float *data);
void phong_shading();

#endif // __LIGHTING_H_GUARD__
