uniform float t;
varying float fvx, fvy;

uniform sampler2D leaf, sky;
uniform bool per_vert;

void main() {

  float vx = gl_Vertex.x; float vy = gl_Vertex.y;
  // not sure if supposed to give 'actual' height or not...
  float height = .2*cos(vx*vx+vy*vy);
  // set position
  gl_Position = gl_ModelViewProjectionMatrix * vec4(vx, vy, height, 1);

  // If the per-vertex normal mode is on, calculate normal 
  if (per_vert) {    
    vec3 N = normalize(vec3(2.*vx*sin(vx*vx + vy*vy + t),
			    2.*vy*sin(vx*vx + vy*vy + t),
			    1.));
      
    // set sky texcoords
    gl_TexCoord[0].xy = vec2((N.x+1)/2, (N.y+1)/2);
    // set leaves texcoords
    gl_TexCoord[1].xy = vec2((vx+5)/10, (vy+5)/10);
  } else {
    // otherwise, just set up varyings for fragment shader to use
    fvx = vx;
    fvy = vy;
  }
}
