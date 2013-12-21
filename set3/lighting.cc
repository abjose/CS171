
#include "lighting.hh"

// TODO: move this to a sensible directory
// TODO: add z-buff stuff...presumable to draw_pixels funcs
// TODO: change some references to shared_ptrs

Matrix<float,3,1> light_func(Matrix<float,3,1> n, Matrix<float,3,1> v,
			     std::shared_ptr<MaterialBlock> material, 
			     //std::vector<LightBlock*> lights, 
			     LightBlock& light, 
			     Matrix<float,3,1> camerapos) {
  // let n = surface normal (nx,ny,nz)
  // let v = point in space (x,y,z)
  // let lights = [light0, light1, ... ]
  // let camerapos = (x,y,z)
  
  Matrix<float,3,1> scolor = material->specular; // (r,g,b)
  Matrix<float,3,1> dcolor = material->diffuse;  // (r,g,b)
  Matrix<float,3,1> acolor = material->ambient;  // (r,g,b)
  float shiny =  material->shininess;     // (a scalar, an exponent >= 0)
    
  // start off the diffuse and specular at pitch black
  Matrix<float,3,1> diffuse  = makeVector<float>(0,0,0);
  Matrix<float,3,1> specular = makeVector<float>(0,0,0);
  // copy the ambient color (for the eyelight extra credit code, you can change 
  // it here to rely on distance from the camera)
  Matrix<float,3,1> ambient = acolor;

  //for (auto& light : lights) {
    // get the light position and color from the light
    // let lx = light position (x,y,z)
    // let lc = light color (r,g,b)
    Matrix<float,3,1> lx = light.location;
    Matrix<float,3,1> lc = light.color;
    
    // first calculate the addition this light makes to the diffuse part
    //Matrix<float,3,1> ddiffuse = zero_clip(lc * (n . unit(lx - v)));
    Matrix<float,3,1> ddiffuse = (lc * (n.dot((lx-v).normalize()))).zero_clip();
    // accumulate that
    diffuse += ddiffuse;
    
    // calculate the specular exponent
    //k = zero_clip(n . unit(unit(camerapos - v) + unit(lx - v)));
    float k = n.dot(((camerapos-v).normalize() +
		     (lx-v).normalize()).normalize());
    if (k < 0) k = 0;
    // calculate the addition to the specular highlight
    // k^shiny is a scalar, lc is (r,g,b)
    Matrix<float,3,1> dspecular = (lc * pow(k,shiny)).zero_clip();
    // accumulate that
    specular += dspecular;
  //}

  // after working on all the lights, clamp the diffuse value to 1
  diffuse.one_clip();
  // note that diffuse, dcolor,specular and scolor are all (r,g,b).
  // * here represents component-wise multiplication
  //rgb = one_clip(ambient + diffuse*dcolor + specular*scolor)
  diffuse[0] *= dcolor[0]; diffuse[1] *= dcolor[1]; diffuse[2] *= dcolor[2];
  specular[0] *= scolor[0]; specular[1] *= scolor[1]; specular[2] *= scolor[2];
  Matrix<float,3,1> rgb = (ambient + diffuse + specular).one_clip();
  return rgb;
}

void draw_pixels_with_constant_color(Canvas<Matrix<float,3,1> > &c, 
				     Matrix<float,3,1> color) {
  // iterate through pixels_to_draw vector
  for (auto& it : pixels_to_draw)
    // NOTE SURE THESE SHOULD BE SCALED!!
    //c.set_pixel(it[0],it[1],color,true);
    c.set_pixel(it[0],it[1],color);
}

void draw_pixels(Canvas<Matrix<float,3,1> >& c) {
  // iterate through pixels_to_draw vector
  float x,y,r,g,b;
  for (auto& it : pixels_to_draw) {
    x = it[0]; y = it[1];
    r = it[2]; g = it[3]; b = it[4];
    // maybe just do this all in other draw fns, have pixels_to_draw be tuples?
    Matrix<float,3,1> color;
    float a[] = {r,g,b};
    color.copy(a);
    //c.set_pixel(x,y,color,true);
    c.set_pixel(x,y,color);
  }
}

void draw_flat(int x, int y, float *data) {
  // could just add to a vector of color - point things to go through
  // in the flat_shading function so don't have to do any funky stuff
  // with making this a class or anything
  // uhhh, should these be scaled later or not?
  pixels_to_draw.push_back(new float[2] {(float) x, (float) y});
}
void flat_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		  Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		  Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		  std::shared_ptr<MaterialBlock> material, 
		  //std::vector<LightBlock*> lights
		  LightBlock& light,
		  CameraBlock& camera, 
		  std::shared_ptr<TransformBlock> transform,
		  Canvas<Matrix<float,3,1> >& c) {
  // TODO: allow multiple lights...
  // Compute the averge location and average normal of each of the 3 
  // vertices. Remember to normalize your normals.
  // sure the normals come from the file?
  Matrix<float,3,1> avg_loc  = (t0 + t1 + t2) / 3;
  Matrix<float,3,1> avg_norm = (n0 + n1 + n2) / 3;
  avg_norm.normalize();

  // TODO: Make it easier to initialize matrices or append indices to them...
  // could just have a constructor that takes an array or something?
  // and/or a constructor that takes another matrix and attemps to copy
  // its contents even if it's not the right size...maybe just for vectors.
 
  // Calculate the corresponding color of the average location and normal 
  // with the lighting function.
  Matrix<float,3,1> color = light_func(avg_norm, avg_loc, 
				       material, light, camera.position);

  // Convert your locations to NDC by applying the perspective and 
  // camera transforms.
  Matrix<float,4,4> final_transform = camera.get_perspective_projection() * 
                                      camera.get_inverse_transform() * 
                                      transform->get_final_transform();
  Matrix<float,4,1> temp_loc;
  float a[] = {avg_loc[0],avg_loc[1],avg_loc[2],1};
  temp_loc.copy(a);
  temp_loc = final_transform * temp_loc;
  temp_loc.homogenize();
  avg_loc[0] = temp_loc[0]; avg_loc[1] = temp_loc[1]; avg_loc[2] = temp_loc[2];

  // TODO: Err, couldn't you just convert to NDC in separator block and skip
  //       a lot of this stuff?
  //       I guess make sure works current way, then modify and see if still
  //       works.

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
}



void draw_gourand(int x, int y, float *data) {
}
void gourand_shading() {
  // Call the lighting function on each of the 3 vertices and their 
  // corresponding normals, to get a unique color for each.

  // Convert your positions to NDC.

  // Call Bill's code with 3 vertices, each with data array = {x, y, z, r, g, b}
  // (since we're interpolating across both location and color). 
  // Then, draw each of these pixels with their corresponding interpolated color
}


void draw_phong(int x, int y, float *data) {
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
