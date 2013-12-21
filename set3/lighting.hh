#ifndef __LIGHTING_H_GUARD__
#define __LIGHTING_H_GUARD__

#include <vector>
#include "raster/raster.h"
#include "matrix/matrix.hh"


// TODO: move this to a sensible directory

// this right?
// vector of x,y,r,g,b floats for drawing pixels into the canvas
static std::vector<float*> pixels_to_draw;


void draw_pixels_with_constant_color(Canvas& c, Matrix<float,3,1> color) {
  // iterate through pixels_to_draw vector
  for (auto& it : pixels_to_draw)
    c->set_pixel(it[0],it[1],color,scale=true);
}

void draw_pixels(Canvas& c) {
  // iterate through pixels_to_draw vector
  float x,y,r,g,b;
  for (auto& it : pixels_to_draw) {
    x = it[0]; y = it[1];
    r = it[2]; g = it[3]; b = it[4];
    // maybe just do this all in other draw fns, have pixels_to_draw be tuples?
    Matrix<float,3,1> color;
    float a[] = {r,g,b};
    color.copy(a);
    c->set_pixel(x,y,color,scale=true);
  }
}

void flat_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		  Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		  Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		  MaterialBlock& material, vector<LightBlock&> lights, 
		  CameraBlock& camera, TransformBlock& transform,
		  Canvas& c) {
  // Compute the averge location and average normal of each of the 3 
  // vertices. Remember to normalize your normals.
  // sure the normals come from the file?
  Matrix<float,3,1> avg_loc  = (t0 + t1 + t2) / 3;
  Matrix<float,3,1> avg_norm = (n0 + n1 + n2) / 3;
  avg_norm.normalize();
 
  // Calculate the corresponding color of the average location and normal 
  // with the lighting function.
  Matrix<float,3,1> color = light_func(avg_norm, avg_loc, 
				       material, lights, camerapos);

  // Convert your locations to NDC by applying the perspective and 
  // camera transforms.
  Matrix<float,4,4> final_transform = camera->get_perspective_projection() * 
                                      camera->get_inverse_transform() * 
                                      transform->get_final_transform();
  avg_loc = final_transform * avg_loc;
  avg_loc.homogenize();

  // Call Bill's code with 3 vertices, each with data array = {x, y, z} 
  // (since we're only interpolating across locations)
  vertex verts[3];
  for (int i = 0; i < 3; i++) verts[i].numData = 3; // just x,y,z
  // TODO: pass input stuff as vectors so can just loop?
  verts[0].data = new float[3] {t0[0], t0[1], t0[2]};
  verts[1].data = new float[3] {t1[0], t1[1], t1[2]};
  verts[2].data = new float[3] {t2[0], t2[1], t2[2]};
  pixels_to_draw.clear();
  raster(verts, draw_flat);

  // Draw pixels from Bill's code for each interpolated location on 
  // the face, and give each vertex the same RGB calculated in step 2.
  draw_pixels_with_constant_color(c, color);

  // TODO: SHOULD PROBABLY MAKE SURE FLAT WORKS BEFORE IMPLEMENTING OTHER ONES!
}

void draw_flat(int x, int y, float *data) {
  // could just add to a vector of color - point things to go through
  // in the flat_shading function so don't have to do any funky stuff
  // with making this a class or anything
  pixels_to_draw.push_back(new float[2] {x,y});
}

void gourand_shading() {
  // Call the lighting function on each of the 3 vertices and their 
  // corresponding normals, to get a unique color for each.

  // Convert your positions to NDC.

  // Call Bill's code with 3 vertices, each with data array = {x, y, z, r, g, b}
  // (since we're interpolating across both location and color). 
  // Then, draw each of these pixels with their corresponding interpolated color
}

void draw_gourand(int x, int y, float *data) {
}


void phong_shading() {
  // Convert your 3 vertex locations to NDC.

  // Call Bill's code with 3 vertices, each with data array = 
  // {x, y, z, nx, ny, nz} (since we're interpolating across both location and 
  // normals).

  // Call the lighting function on each of the new pixel locations and their
  // corresponding interpolated location to get the calculated color,
  // and draw that pixel.
}

void draw_phong(int x, int y, float *data) {
}

/*
5 things needed by the Lighting Function
----------------------------------------
Changes from call to call
  1 pos     point in world space*
  2 norm    normal in world space*

Same from polygon to polygon
  3 material* for that polygon (that Separator, actually)

Same for all shading calls (unless camera/lights move)
  4 all the lights*    in world space
  5 camera position    in world space

NOTES: 1: No frustrum/camera position applied,
          only object Transform blocks (obj->world space)
       2: Normals are transformed by transpose of inverse
          of O matrix, without translations. That is, if 
          my separator has T, R, and S, I will translate
          the normal by doing N_new = Transpose(Inverse(R * S)) * N.
          See the webpage on Homogeneous Coordinates and Transformation
          Matrices. You must always remember to normalize your normal
          (hence the term normalize).
       3: Material consists of diffuse, ambient, and
          specular/shininess exponent
       4: Each light has x,y,z (position) and r,g,b (intensity)
*/


// some pseudo-code to do the lighting
(r,g,b) light_func(n, v, material, lights, camerapos) {
//MaterialBlock& material, vector<LightBlock&> lights, CameraBlock& camera
  // let n = surface normal (nx,ny,nz)
  // let v = point in space (x,y,z)
  // let lights = [light0, light1, ... ]
  // let camerapos = (x,y,z)
  // maybe just pass matrix instead of CameraBlock ref? ehhh...
  
  scolor = material.specularcolor; // (r,g,b)
  dcolor = material.diffusecolor;  // (r,g,b)
  acolor = material.ambientcolor;  // (r,g,b)
  shiny =  material.shininess;     // (a scalar, an exponent >= 0)
    
  // start off the diffuse and specular
  // at pitch black
  diffuse = [0.0, 0.0, 0.0];
  specular = [0.0, 0.0, 0.0];
  // copy the ambient color (for the eyelight ex/cred
  // code, you can change it here to rely on distance
  // from the camera)
  ambient = acolor;

  for (l in lights) {
    // get the light position and color from the light
    // let lx = light position (x,y,z)
    // let lc = light color (r,g,b)
    lx, lc = l;
    
    // first calculate the addition this light makes
    // to the diffuse part
    ddiffuse = zero_clip(lc * (n . unit(lx - v)));
    // accumulate that
    diffuse += ddiffuse;
    
    // calculate the specular exponent
    k = zero_clip(n . unit(unit(camerapos - v) + unit(lx - v)));
    // calculate the addition to the specular highlight
    // k^shiny is a scalar, lc is (r,g,b)
    dspecular = zero_clip(k^shiny * lc);
    // acumulate that
    specular += dspecular;
  }

  // after working on all the lights, clamp the diffuse value to 1
  d = one_clip(diffuse);
  // note that d,dcolor,specular and scolor are all (r,g,b).
  // * here represents component-wise multiplication
  rgb = one_clip(ambient + d*dcolor + specular*scolor);
  return rgb;
}



#endif // __LIGHTING_H_GUARD__
