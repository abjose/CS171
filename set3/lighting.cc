
#include "lighting.hh"

// TODO: move this to a sensible directory
// TODO: lighting seems to be on...the opposite of the right side?
//       does anything need to be done to lighting to put into world coords?
// TODO: change vertex variables to 'v' instead of 't'?
// TODO: IMPLEMENT BACKFACE CULLING, NORMAL TRIANGLING...
//       THENNNN WORK ON LIGHTING PROBLEMS


Matrix<float,3,1> light_func(Matrix<float,3,1> n, Matrix<float,3,1> v,
			     std::shared_ptr<MaterialBlock> material, 
			     std::vector<std::shared_ptr<LightBlock> > lights, 
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
  Matrix<float,3,1> diffuse  = makeVector3<float>(0,0,0);
  Matrix<float,3,1> specular = makeVector3<float>(0,0,0);
  // copy the ambient color (for the eyelight extra credit code, you can change 
  // it here to rely on distance from the camera)
  Matrix<float,3,1> ambient = acolor;

  for (auto& light : lights) {
    // get the light position and color from the light
    // let lx = light position (x,y,z)
    // let lc = light color (r,g,b)
    Matrix<float,3,1> lx = light->location;
    Matrix<float,3,1> lc = light->color;
    
    // first calculate the addition this light makes to the diffuse part
    //Matrix<float,3,1> ddiffuse = zero_clip(lc * (n . unit(lx - v)));
    Matrix<float,3,1> ddiffuse = (lc * (n.dot((v-lx).normalize()))).zero_clip();
    // accumulate that
    diffuse += ddiffuse;
    //std::cout << "diffuse:\n" << ddiffuse << std::endl;
    
    // calculate the specular exponent
    //k = zero_clip(n . unit(unit(camerapos - v) + unit(lx - v)));
    float k = n.dot(((camerapos - v).normalize() +
		     (lx - v).normalize()).normalize());
    if (k < 0) k = 0;
    //std::cout << "k: " << k << std::endl; 
    //std::cout << "v: " << v << std::endl;
    //std::cout << "pow: " << pow(k,shiny) << std::endl;
    //std::cout << "\n\n\n";
    // calculate the addition to the specular highlight
    // k^shiny is a scalar, lc is (r,g,b)
    Matrix<float,3,1> dspecular = (lc * pow(k,shiny)).zero_clip();
    // accumulate that
    specular += dspecular;
    //std::cout << "specular:\n" << dspecular << std::endl;
  }
  
  //std::cout << specular << std::endl;

  // after working on all the lights, clamp the diffuse value to 1
  diffuse = diffuse.one_clip();
  // note that diffuse, dcolor,specular and scolor are all (r,g,b).
  // * here represents component-wise multiplication
  //rgb = one_clip(ambient + diffuse*dcolor + specular*scolor)
  diffuse[0]  *= dcolor[0]; diffuse[1]  *= dcolor[1]; diffuse[2]  *= dcolor[2];
  specular[0] *= scolor[0]; specular[1] *= scolor[1]; specular[2] *= scolor[2];
  Matrix<float,3,1> rgb = (ambient + diffuse + specular).one_clip();
  return rgb;
}

void draw_pixels_with_constant_color(std::shared_ptr<Canvas> c, 
				     Matrix<float,3,1> color) {
  // iterate through pixels_to_draw vector
  for (auto& it : pixels_to_draw) {
    c->set_pixel((int) it[0], (int) it[1], color);
  }
}

void draw_pixels(std::shared_ptr<Canvas> c) {
  // iterate through pixels_to_draw vector
  for (auto& it : pixels_to_draw) {
    //std::cout << it[2] << ", " << it[3] << ", " << it[4] << std::endl;
    c->set_pixel(it[0],it[1], makeVector3<float>(it[2],it[3],it[4]));
  }
}

void draw_pixels(std::shared_ptr<Canvas> c,
		 std::shared_ptr<MaterialBlock> material, 
		 std::vector<std::shared_ptr<LightBlock> > lights,
		 std::shared_ptr<CameraBlock> camera) {
  // iterate through pixels_to_draw vector
  for (auto& it : pixels_to_draw) {
    // it = {x,y, x0,y0,z0, nx,ny,nz}
    // x0 y0 z0 are in world space
    auto v = makeVector3<float>(it[2],it[3],it[4]);
    auto n = makeVector3<float>(it[5],it[6],it[7]).normalize();
    auto color = light_func(n, v, material, lights, camera->position);
    c->set_pixel(it[0],it[1], color);
  }
}

void draw_flat(int x, int y, float *data) {
  // should move this z-buff stuff into its own function when you know it works
  auto xy = std::make_pair(x,y);
  float z = data[2];
  if(z_buff.count(xy) == 0 || z < z_buff[xy]) {
    z_buff[xy] = z;
    pixels_to_draw.push_back(new float[2] {(float) x, (float) y});
  } 
}
void flat_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		  Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		  Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		  std::shared_ptr<MaterialBlock> material, 
		  std::vector<std::shared_ptr<LightBlock> > lights,
		  std::shared_ptr<CameraBlock> camera, 
		  std::shared_ptr<TransformBlock> transform,
		  std::shared_ptr<Canvas> c) {
  // Compute the averge location and average normal of each of the 3 
  // vertices. Remember to normalize your normals.
  auto avg_loc  = (t0 + t1 + t2) / 3;
  auto avg_norm = ((n0 + n1 + n2) / 3).normalize();
  
  // Calculate the corresponding color of the average location and normal 
  // with the lighting function.
  auto color = light_func(avg_norm,avg_loc, material, lights, camera->position);

  // Convert your locations to NDC by applying the perspective and 
  // camera transforms.
  Matrix<float,4,4> T = camera->get_perspective_projection() * 
    camera->get_inverse_transform();
  auto t0_4 = (T * makeVector4<float>(t0[0],t0[1],t0[2],1)).homogenize();
  auto t1_4 = (T * makeVector4<float>(t1[0],t1[1],t1[2],1)).homogenize();
  auto t2_4 = (T * makeVector4<float>(t2[0],t2[1],t2[2],1)).homogenize();

  // Call Bill's code with 3 vertices, each with data array = {x, y, z} 
  // (since we're only interpolating across locations)
  vertex verts[3];
  for (int i = 0; i < 3; i++) verts[i].numData = 3; // just x,y,z
  // TODO: pass input stuff as vectors so can just loop?
  verts[0].data = new float[3] {t0_4[0], t0_4[1], t0_4[2]};
  verts[1].data = new float[3] {t1_4[0], t1_4[1], t1_4[2]};
  verts[2].data = new float[3] {t2_4[0], t2_4[1], t2_4[2]};
  pixels_to_draw.clear();
  raster(verts, draw_flat);

  // Draw pixels from Bill's code for each interpolated location on 
  // the face, and give each vertex the same RGB calculated in step 2.
  draw_pixels_with_constant_color(c, color);
}


void draw_gouraud(int x, int y, float *data) {
  // should move this z-buff stuff into its own function when you know it works
  auto xy = std::make_pair(x,y);
  float z = data[2];
  if(z_buff.count(xy) == 0 || z < z_buff[xy]) {
    z_buff[xy] = z;
    pixels_to_draw.push_back(new float[5] 
			     {(float) x, (float) y, data[3], data[4], data[5]});
  } 
}
void gouraud_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		     Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		     Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		     std::shared_ptr<MaterialBlock> material, 
		     std::vector<std::shared_ptr<LightBlock> > lights,
		     std::shared_ptr<CameraBlock> camera, 
		     std::shared_ptr<TransformBlock> transform,
		     std::shared_ptr<Canvas> c) {
  // Call the lighting function on each of the 3 vertices and their 
  // corresponding normals, to get a unique color for each.
  auto c0 = light_func(n0, t0, material, lights, camera->position);
  auto c1 = light_func(n1, t1, material, lights, camera->position);
  auto c2 = light_func(n2, t2, material, lights, camera->position);

  // Convert your positions to NDC.
  Matrix<float,4,4> T = camera->get_perspective_projection() * 
    camera->get_inverse_transform();
  auto t0_4 = (T * makeVector4<float>(t0[0],t0[1],t0[2],1)).homogenize();
  auto t1_4 = (T * makeVector4<float>(t1[0],t1[1],t1[2],1)).homogenize();
  auto t2_4 = (T * makeVector4<float>(t2[0],t2[1],t2[2],1)).homogenize();

  // Call Bill's code with 3 vertices, each with data array = {x, y, z, r, g, b}
  // (since we're interpolating across both location and color). 
  vertex verts[6];
  for (int i = 0; i < 3; i++) verts[i].numData = 6; // x,y,z,r,g,b
  verts[0].data = new float[6] {t0_4[0], t0_4[1], t0_4[2], c0[0], c0[1], c0[2]};
  verts[1].data = new float[6] {t1_4[0], t1_4[1], t1_4[2], c1[0], c1[1], c1[2]};
  verts[2].data = new float[6] {t2_4[0], t2_4[1], t2_4[2], c2[0], c2[1], c2[2]};
  pixels_to_draw.clear();
  raster(verts, draw_gouraud);

  // Then, draw each of these pixels with their corresponding interpolated color
  draw_pixels(c);
}


void draw_phong(int x, int y, float *data) {
  // should move this z-buff stuff into its own function when you know it works
  auto xy = std::make_pair(x,y);
  float z = data[2];
  if(z_buff.count(xy) == 0 || z < z_buff[xy]) {
    z_buff[xy] = z;
    pixels_to_draw.push_back(new float[8] 
			     {(float) x, (float) y, 
				 //data[0], data[1], data[2],
				 data[3], data[4], data[5],
				 data[6], data[7], data[8]});
  } 
}
void phong_shading(Matrix<float,3,1> t0, Matrix<float,3,1> n0,
		   Matrix<float,3,1> t1, Matrix<float,3,1> n1,
		   Matrix<float,3,1> t2, Matrix<float,3,1> n2,
		   std::shared_ptr<MaterialBlock> material, 
		   std::vector<std::shared_ptr<LightBlock> > lights,
		   std::shared_ptr<CameraBlock> camera, 
		   std::shared_ptr<TransformBlock> transform,
		   std::shared_ptr<Canvas> c) {
  // Convert your 3 vertex locations to NDC.
  Matrix<float,4,4> T = camera->get_perspective_projection() * 
    camera->get_inverse_transform();
  auto t0_4 = (T * makeVector4<float>(t0[0],t0[1],t0[2],1)).homogenize();
  auto t1_4 = (T * makeVector4<float>(t1[0],t1[1],t1[2],1)).homogenize();
  auto t2_4 = (T * makeVector4<float>(t2[0],t2[1],t2[2],1)).homogenize();

  // Call Bill's code with 3 vertices, each with data array = 
  // {x, y, z, nx, ny, nz} (since we're interpolating across both location and 
  // normals).
  vertex verts[3];
  for (int i = 0; i < 3; i++) verts[i].numData = 9; // x,y,z,nx,ny,nz
  verts[0].data = new float[9] {t0_4[0], t0_4[1], t0_4[2], 
				t0[0], t0[1], t0[2],
				n0[0], n0[1], n0[2]};
  verts[1].data = new float[9] {t1_4[0], t1_4[1], t1_4[2], 
				t1[0], t1[1], t1[2],
				n1[0], n1[1], n1[2]};
  verts[2].data = new float[9] {t2_4[0], t2_4[1], t2_4[2], 
				t2[0], t2[1], t2[2],
				n2[0], n2[1], n2[2]};
  pixels_to_draw.clear();
  raster(verts, draw_phong);

  // Call the lighting function on each of the new pixel locations and their
  // corresponding interpolated location to get the calculated color,
  // and draw that pixel.
  // NOTE: make sure to pass world loc to lighting function
  draw_pixels(c, material, lights, camera);
}
