uniform float t;
varying float fvx, fvy;

uniform sampler2D leaf, sky;
uniform bool per_vert;

void main() {

  vec4 sky_col;
  vec4 leaf_col;

  // If per-vertex normal mode is off, calculate normal
  if (!per_vert) { 
    vec3 N = normalize(vec3(2.*fvx*sin(fvx*fvx + fvy*fvy + t),
			    2.*fvy*sin(fvx*fvx + fvy*fvy + t),
			    1.));

    sky_col  = texture2D(sky,  vec2((N.x+1)/2, (N.y+1)/2));
    leaf_col = texture2D(leaf, vec2((fvx+5)/10, (fvy+5)/10));
  } else {

    // Get colors using texture coordinate
    sky_col  = texture2D(sky,  gl_TexCoord[0].xy);
    leaf_col = texture2D(leaf, gl_TexCoord[1].xy);

  }

  // Blend two textures together using leaf alpha 
  gl_FragColor = (leaf_col.a * leaf_col + (1.-leaf_col.a) * sky_col);
}
