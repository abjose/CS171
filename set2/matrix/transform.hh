/*
 * Note: the structure of this code (and some of the code itself) follows Steven
 * Mueller's minmatrix library closely.
 */

#ifndef __TRANSFORMS_H_GUARD__
#define __TRANSFORMS_H_GUARD__

#include <math.h>
#include "matrix.hh"

//------------------------------------------
// vector-making convenience stuff
//------------------------------------------

template<typename T>
Matrix<T,3,1> makeVector(T e1, T e2, T e3) {
  Matrix<T,3,1> m;
  T a[] = {e1,e2,e3};
  m.copy(a);
  return m;
}

//------------------------------------------
// 4x4 transformations
//------------------------------------------

template<typename T>
Matrix<T,4,4> translation_matrix(T x, T y, T z) {
  Matrix<T,4,4> res;
  res.make_identity();
  res(0,3) = x;
  res(1,3) = y;
  res(2,3) = z;
  return res;
}

template<typename T>
Matrix<T,4,4> scale_matrix(T a, T b, T c) {
  Matrix<T,4,4> res;
  res.clear(0);
  res(0,0) = a;
  res(1,1) = b;
  res(2,2) = c;
  res(3,3) = 1;
  return res;
}

template<typename T>
Matrix<T,4,4> rotation_matrix(T x, T y, T z, T theta) {
  assert(x != 0 || y != 0 || z != 0);
  Matrix<T,4,4> res;
  res.clear(0);
  // normalize input axis
  Matrix<T,3,1> n = makeVector<float>(x,y,z).normalize();
  x = n[0]; y = n[1]; z = n[2];
  T x_2 = x*x;
  T y_2 = y*y;
  T z_2 = z*z;
  T sin_th = sin(theta);
  T cos_th = cos(theta);
  res(0,0) = x_2 + (1.-x_2)*cos_th;
  res(1,1) = y_2 + (1.-y_2)*cos_th;
  res(2,2) = z_2 + (1.-z_2)*cos_th;
  res(3,3) = 1;
  res(0,1) = x*y*(1.-cos_th) - z*sin_th;
  res(0,2) = x*z*(1.-cos_th) + y*sin_th;
  res(1,0) = x*y*(1.-cos_th) + z*sin_th;
  res(1,2) = y*z*(1.-cos_th) - x*sin_th;
  res(2,0) = x*z*(1.-cos_th) - y*sin_th;
  res(2,1) = y*z*(1.-cos_th) + x*sin_th;
  return res;
}
  

#endif // __TRANSFORMS_H_GUARD__
