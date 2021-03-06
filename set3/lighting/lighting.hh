#ifndef __LIGHTING_H_GUARD__
#define __LIGHTING_H_GUARD__

#include <vector>
#include <memory>
#include <map>
#include <utility>
#include "../blocks/light_block.hh"
#include "../blocks/material_block.hh"
#include "../blocks/camera_block.hh"
#include "../matrix/canvas.hh"

// vector of float arrays for drawing pixels onto the canvas
static std::vector<float*> pixels_to_draw;
// zbuffer map
static std::map<std::pair<int,int>,float> z_buff;
// camera...for clipping...should probably just go ahead and make this a class..
static std::shared_ptr<CameraBlock> cam;

Matrix<float,3,1> light_func(Matrix<float,3,1> n, Matrix<float,3,1> v,
			     std::shared_ptr<MaterialBlock> material, 
			     std::vector<std::shared_ptr<LightBlock> > lights, 
			     std::shared_ptr<LightBlock> light, 
			     Matrix<float,3,1> camerapos);

void draw_pixels_with_constant_color(std::shared_ptr<Canvas> c, 
				     Matrix<float,3,1> color);
void draw_pixels(std::shared_ptr<Canvas> c,
		 std::shared_ptr<MaterialBlock> material, 
		 std::vector<std::shared_ptr<LightBlock> > lights,
		 std::shared_ptr<CameraBlock> camera);
void draw_pixels(std::shared_ptr<Canvas> c);

void draw_flat(int x, int y, float *data);
void flat_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		  Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		  Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		  std::shared_ptr<MaterialBlock> material, 
		  std::vector<std::shared_ptr<LightBlock> > lights,
		  std::shared_ptr<CameraBlock> camera, 
		  std::shared_ptr<Canvas> c);

void gouraud_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		     Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		     Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		     std::shared_ptr<MaterialBlock> material, 
		     std::vector<std::shared_ptr<LightBlock> > lights,
		     std::shared_ptr<CameraBlock> camera, 
		     std::shared_ptr<Canvas> c);
void gouraud_shading();

void draw_phong(int x, int y, float *data);
void phong_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		   Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		   Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		   std::shared_ptr<MaterialBlock> material, 
		   std::vector<std::shared_ptr<LightBlock> > lights,
		   std::shared_ptr<CameraBlock> camera, 
		   std::shared_ptr<Canvas> c);

#endif // __LIGHTING_H_GUARD__
