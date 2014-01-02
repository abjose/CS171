
#include <iostream>

float Spline::B_i(int i, float u) {
  switch(i) {
  case 0: return pow(1-u, 3) / 6;
  case 1: return (pow(3*u,3) - pow(6*u,2) + 4) / 6;
  case 2: return (pow(-3*u,3) + pow(3*u,2) + 3*u + 1) / 6;
  case 3: return pow(u,3) / 6;
  }
}

float Spline::N(int i, float u) {
  // do with dynamic programming instead!
  if (k==1) {
    if (t[i] <= u && u <= t[i+1])
      return 1;
    return 0;
  }
  float l = 0; float r = 0;
  l_d = t[i+k-1] - t[i];
  r_d = t[i+k] - t[i+1];
  if (l_d != 0)
    l = (u-t[i])*N(i,k-1,u) / l_d;
  if (r_d != 0)
    r = (t[i+k]-u)*N(i+1,k-1,u) / r_d;
  return l+r;
}

//float N_dynamic(i, u) {}

CtrlPt Spline::Q(float u) {
  // NOTE: where is U vs u = U-i? If things don't look right should verify...
  CtrlPt sum = makeVector3<float>(0,0,0);
  for (int i=0; i<k; i++)
    sum += p[i] * N(i, u);
  return sum;
}

float Spline::a(int i, int j, float t_new) {
  // j should be the index of the knot just to the left of the new one
  if (1 <= i && i <= j-k) return 1;
  if (j+1 <= i && i <= n) return 0;
  if (j-k+1 <= i && i <= j) return (t_new - t[i]) / (t[i+k] - t[i]);
  std::cout << "Something weird happend in spline::a\n";
  return -1;
}

int Spline::find_knot_index(float u) {
  // given u, find index to insert new knot s.t. t_prev < t_new <= t_next
  int imin = 0; int imax = t.size();
  while (imax >= imin) {
    // calculate the midpoint
    int imid = (imin+imax) / 2;
    if(t[imid-1] < u && u <= t[imid]) return imid; 
    else if (t[imid] < u) imin = imid + 1;
    else                  imax = imid - 1;
  }
  // key was not found
  std::cout << "Uh oh, key not foundin Spline::find_knot_index\n";
  return -1;
}

void Spline::insert_knot(float t_new) {
  // find indices of surrounding knots
  int t_next = find_knot_index(t_new);
  if (t_next == t.size()) std::cout << "Uh oh, nothing found\n";
  if (t_next == 0)        std::cout << "Uh oh, found 0th element\n";
  int t_prev = t_next-1;

  // insert knot...sure this is supposed to happen before getting a's?
  t.insert(t.begin()+t_next, t_new);

  // find new control points
  // insert new control point in relevant range...should be last?
  // (3) Fill in the new control point list so that the control 
  //     point values are affected only by old control point 
  //     values (but NEW knot values).
  int j = t_prev;
  // create new control point list
  std::vector<CtrlPt> new_p;
  // really have to reassign everything?
  float a_i;
  p_new.push_back(p[0]); // p'_0 = p_0
  for (int i=1; i<p.size(); i++) {
    a_i = a(i, t_prev, t_new);
    p_new.push_back((1.-a_i)*p[i-1] + a_i*p[i]);
  }
  p_new.push_back(p.back()); // p'_n+1 = p_n
  p = p_new;

  // p.insert(p.begin()+j, makeVector3<float>(0,0,0));
  // for (i in range(j-k+1,j+1)) {
  //   a_i  = a(i,k,t_prev, t_new);
  //   p[i] = (1.-a_i)*p[i-1] + a_i*p[i];
  // }
}

// need to add something that goes from x,y click to U value, then inserts
// knot
// so basically a version of insert_knot that takes x,y
float Spline::get_knot_from_pt(int x, int y) {
  // TODO: could speed this up a lot
  // TODO: NEED TO SCALE PTS
  auto click = makeVector2<float>(x,y);
  float best_d = 999999999.;
  int best_i = 0;
  float d;
  for (int i=0; i<spline.size(); i++) {
    d = (click-spline[i]).magnitude();
    if (d < best_d) {
      best_d = d;
      best_i = i;
    }
  }
  return best_i / spline.size();
}

void Spine::insert_knot(int x, int y) {
  insert_knot(get_knot_from_pt(x,y));
}

// populate spline vector with points according to resolution
// TODO: NEED TO SCALE FINAL POINTS!!!!
void Spline::make_spline() {
  float du = 1. / res;
  CtrlPt pt;
  spline.clear();
  for (float u=0; u<=1.0; u+=du) {
    pt = Q(u).homogenize(); // necessary to homogenize?
    spline.push_back(makeVector2<float>(pt[0], pt[1]));
  }
}
