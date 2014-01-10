uniform float t;
const float pi = 3.1415926535;

uniform sampler2D leaf, sky;

varying float u, v;

void main() {
  /*
  // Convert from the [-5,5]x[-5,5] range provided into radians
  // between 0 and 2*theta
  u = (gl_Vertex.x + 5.0) / 10.0 * 2 * pi;
  v = (gl_Vertex.y + 5.0) / 10.0 * 2 * pi;
  float r = sin(4*u+t)/4+1;
  float R = cos(6*cos(u)-t)/4+3;

  float a = R+r*cos(v);
  vec3 world = vec3(a*cos(u), a*sin(u), r*sin(v));

  gl_Position = gl_ModelViewProjectionMatrix * vec4(world,1.0);
  */



  /* Compute transformation of each point  */
  // just use gl_ModelViewMatrix?


  /* Calculate height based on time and distance from center  */


  /* If the per-vertex normal mode is on, calculate normal  */
  /*
  N1 = vec3(2.*x*sin(x*x + y*y), 
	    2.*y*sin(x*x + y*y), 
	    1.);
  N2 = vec3(2.*x*sin(x*x + prev_y*prev_y), 
	    2.*prev_y*sin(x*x + prev_y*prev_y), 
	    1.);
  */


  /* Set the texture coordinate */





}
