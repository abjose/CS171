/*
 * Note: the structure of this code (and some of the code itself) follows Steven
 * Mueller's minmatrix library closely.
 */

#ifndef __MINMATRIX_H_GUARD__
#define __MINMATRIX_H_GUARD__


#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

template<typename T, int R, int C>
class Matrix {

private:
  T matrix[R*C]; // row-major

public:
  // return size
  int size() const { return R*C; }
  int nrows() const { return R; }
  int ncols() const { return C; }
  
  // directly copy elements from an array
  void copy(const T *arr) {
    for(int i=0; i < size(); i++)
      matrix[i] = arr[i];
  }

  
  //------------------------------------------
  // constructors
  //------------------------------------------

  // Default constructor
  Matrix() {}  

  // Destructor
  ~Matrix() {}

  // Copy constructor - make deep copy
  Matrix(const Matrix& m2) { 
    copy(m2.matrix); 
  }

  // = operator for assignment
  Matrix &operator=(const Matrix &m2) {
    if (this != &m2)
      copy(m2.matrix);
    return *this;
  }

  // clear matrix and set elements to 'val'
  void clear(T val) { 
    for (int i=0; i < size(); i++) 
      matrix[i]=val; 
  }


  //------------------------------------------
  // indexing
  //------------------------------------------

  // return true if r,c in matrix bounds
  bool in_bounds(int r, int c) const {
    return r >= 0 && c >= 0 && r < R && c < C;
  }
  // Return true if i is in bounds (accessing as a vector)
  bool in_bounds(int i) const {
    return i < size();
  }

  // nonchecked subscripting get/set
  const T &operator()(int r, int c) const { 
    return matrix[r*C+c]; 
  }
  T &operator()(int r, int c) { 
    return matrix[r*C+c]; 
  }
  // non-checked accessor
  const T &ref(int row, int col) const { return matrix[col + row*C]; }
  // non-checked LValue setter
  T &ref(int row, int col) { return matrix[col + row*C]; }

  // flat addressing
  const T &operator[](int i) const { 
    assert(in_bounds(i)); 
    return matrix[i]; 
  }
  T &operator[](int i) { 
    assert(in_bounds(i)); 
    return matrix[i]; 
  }
  // to make accessing nicer here
  const T &ref(int i) const { return (*this)[i]; }
  T &ref(int i) { return (*this)[i]; }


  //------------------------------------------
  // display
  //------------------------------------------

  void display() const {
    std::cout << R << "x" << C << " matrix:" << std::endl;
    for (int r=0; r<R; r++) {
      for (int c=0; c<C; c++)
	std::cout << ref(r,c) << " ";
      std::cout << std::endl;
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {  
    os << R << "x" << C << " matrix:\n";
    for (int r=0; r<R; r++) {
      for (int c=0; c<C; c++)
	os << m(r,c) << " ";
      os << std::endl;
    }
    return os;
  }


  //------------------------------------------
  // math-y things
  //------------------------------------------

  // TODO: add an is_vector test?

  T dot(const Matrix<T,R,1> &other) {
    assert(R==1 || C==1);
    T d = 0;
    for (int i=0; i < size(); i++)
      d += ref(i)*other.ref(i);
    return d;
  }

  // TODO: hmm, should this always be a float/
  T magnitude() const {
    assert(R==1 || C==1);
    T mag = 0;
    for (int i=0; i < size(); i++)
      mag += ref(i)*ref(i);
    return sqrt(mag);
  }

  void homogenize() {
    // Kinda gross. Assumes x,y,z,w.
    assert(R==1 || C==1);
    assert(size() == 4);
    T w = ref(3);
    *this /= w;
  }

  Matrix normalize() {
    //void normalize() {
    T mag = magnitude();
    //assert(mag != 0);
    // should just make 0 vector if magnitude is 0?
    if (abs(mag) < 0.000001)
      clear(0);
    else
      *this /= mag;
    return *this;
  }

  // return tranpose as new matrix
  //Matrix transpose() {
  Matrix<T,C,R> transpose() {
    Matrix<T,C,R> res;
    for (int row=0; row<R; row++)
	for (int col=0; col<C; col++)
	    res.ref(col,row) = ref(row,col);
    return res;
  }
  
  void set_diagonal(const Matrix<T,R,1> &diag) {
    // don't reallly have to assert square...
    assert(R==C);
    clear(0);
    for (int i=0; i < R; i++) 
      ref(i,i) = diag[i];
  }

  void make_identity() {
    assert(R==C);
    clear(0);
    for (int i=0; i < R; i++) 
      ref(i,i) = 1;
  }

  void zero_clip() {
    for (int i=0; i < size(); i++)
      if (ref(i) < 0) ref(i) = 0;
  }

  void one_clip() {
    for (int i=0; i < size(); i++)
      if (ref(i) > 1) ref(i) = 1;
  }  

  //------------------------------------------
  // scalar arithmetic
  //------------------------------------------

  // operator+=(scalar), etc.
#define MAKE_MATRIX_opeq_SCALAR(op, impop)	\
  Matrix &op (T scalar) {			\
    for (int i=0; i < size(); i++)		\
      ref(i) impop scalar;			\
    return *this;				\
  }

  MAKE_MATRIX_opeq_SCALAR(operator+=, +=)
  MAKE_MATRIX_opeq_SCALAR(operator-=, -=)
  MAKE_MATRIX_opeq_SCALAR(operator*=, *=)
  MAKE_MATRIX_opeq_SCALAR(operator/=, /=)
  
  // operator+(scalar), etc.
#define MAKE_MATRIX_op_SCALAR(op, impop)	\
  const Matrix op(const T scalar) const {	\
    return Matrix(*this) impop scalar;		\
  }

  MAKE_MATRIX_op_SCALAR(operator+, +=)
  MAKE_MATRIX_op_SCALAR(operator-, -=)
  MAKE_MATRIX_op_SCALAR(operator*, *=)
  MAKE_MATRIX_op_SCALAR(operator/, /=)


  //------------------------------------------
  // matrix operations
  //------------------------------------------

  // element-wise addition/subtraction
  Matrix &operator+=(const Matrix &m2) {		
    for (int i=0; i < size(); i++) 
      ref(i) += m2[i];
    return *this;
  }
  const Matrix operator+(const Matrix m2) const {
    return Matrix(*this) += m2;
  }
  Matrix &operator-=(const Matrix &m2) {		
    for (int i=0; i < size(); i++) 
      ref(i) += m2[i];
    return *this;
  }
  const Matrix operator-(const Matrix m2) const {
    return Matrix(*this) -= m2;
  }

  // THIS SHOULD THROW BETTER ERRORS - NOT VERY GOOD WHEN 
  // trying to assign result to matrix of the wrong size
  // matrix multiplication
  // will this allow multiplication by matrices of other sizes?
  template<int C2>
  const Matrix<T,R,C2> operator*(const Matrix<T,C,C2> &m2) const {
    Matrix<T,R,C2> res;
    for (int r=0; r<R; r++)
      for (int c=0; c<C2; c++) {
	T dot = 0;
	for (int i=0; i<C; i++)
	  dot += ref(r,i) * m2(i,c);
	res(r,c) = dot;
      }
    return res;
  }

  void apply_transform_stack(const std::vector<Matrix<T,4,4> > ts) {
    assert(R==4 && C==1); // awk
    for (auto rit = ts.rbegin(); rit != ts.rend(); ++rit) {
      //(*rit).display();
      *this = (*rit) * (*this);
    }
  }

  // 4x4 matrix inversion....gag. Stolen from: 
  // http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
  //bool invert(const double m[16], double invOut[16]) {
  bool inverse() {
    assert(R==4 && C==4);

    //double inv[16], det;
    double det;
    Matrix inv;
    int i;

    inv[0] = ref(5)  * ref(10) * ref(15) - 
             ref(5)  * ref(11) * ref(14) - 
             ref(9)  * ref(6)  * ref(15) + 
             ref(9)  * ref(7)  * ref(14) +
             ref(13) * ref(6)  * ref(11) - 
             ref(13) * ref(7)  * ref(10);

    inv[4] = -ref(4)  * ref(10) * ref(15) + 
              ref(4)  * ref(11) * ref(14) + 
              ref(8)  * ref(6)  * ref(15) - 
              ref(8)  * ref(7)  * ref(14) - 
              ref(12) * ref(6)  * ref(11) + 
              ref(12) * ref(7)  * ref(10);

    inv[8] = ref(4)  * ref(9) * ref(15) - 
             ref(4)  * ref(11) * ref(13) - 
             ref(8)  * ref(5) * ref(15) + 
             ref(8)  * ref(7) * ref(13) + 
             ref(12) * ref(5) * ref(11) - 
             ref(12) * ref(7) * ref(9);

    inv[12] = -ref(4)  * ref(9) * ref(14) + 
               ref(4)  * ref(10) * ref(13) +
               ref(8)  * ref(5) * ref(14) - 
               ref(8)  * ref(6) * ref(13) - 
               ref(12) * ref(5) * ref(10) + 
               ref(12) * ref(6) * ref(9);

    inv[1] = -ref(1)  * ref(10) * ref(15) + 
              ref(1)  * ref(11) * ref(14) + 
              ref(9)  * ref(2) * ref(15) - 
              ref(9)  * ref(3) * ref(14) - 
              ref(13) * ref(2) * ref(11) + 
              ref(13) * ref(3) * ref(10);

    inv[5] = ref(0)  * ref(10) * ref(15) - 
             ref(0)  * ref(11) * ref(14) - 
             ref(8)  * ref(2) * ref(15) + 
             ref(8)  * ref(3) * ref(14) + 
             ref(12) * ref(2) * ref(11) - 
             ref(12) * ref(3) * ref(10);

    inv[9] = -ref(0)  * ref(9) * ref(15) + 
              ref(0)  * ref(11) * ref(13) + 
              ref(8)  * ref(1) * ref(15) - 
              ref(8)  * ref(3) * ref(13) - 
              ref(12) * ref(1) * ref(11) + 
              ref(12) * ref(3) * ref(9);

    inv[13] = ref(0)  * ref(9) * ref(14) - 
              ref(0)  * ref(10) * ref(13) - 
              ref(8)  * ref(1) * ref(14) + 
              ref(8)  * ref(2) * ref(13) + 
              ref(12) * ref(1) * ref(10) - 
              ref(12) * ref(2) * ref(9);

    inv[2] = ref(1)  * ref(6) * ref(15) - 
             ref(1)  * ref(7) * ref(14) - 
             ref(5)  * ref(2) * ref(15) + 
             ref(5)  * ref(3) * ref(14) + 
             ref(13) * ref(2) * ref(7) - 
             ref(13) * ref(3) * ref(6);

    inv[6] = -ref(0)  * ref(6) * ref(15) + 
              ref(0)  * ref(7) * ref(14) + 
              ref(4)  * ref(2) * ref(15) - 
              ref(4)  * ref(3) * ref(14) - 
              ref(12) * ref(2) * ref(7) + 
              ref(12) * ref(3) * ref(6);

    inv[10] = ref(0)  * ref(5) * ref(15) - 
              ref(0)  * ref(7) * ref(13) - 
              ref(4)  * ref(1) * ref(15) + 
              ref(4)  * ref(3) * ref(13) + 
              ref(12) * ref(1) * ref(7) - 
              ref(12) * ref(3) * ref(5);

    inv[14] = -ref(0)  * ref(5) * ref(14) + 
               ref(0)  * ref(6) * ref(13) + 
               ref(4)  * ref(1) * ref(14) - 
               ref(4)  * ref(2) * ref(13) - 
               ref(12) * ref(1) * ref(6) + 
               ref(12) * ref(2) * ref(5);

    inv[3] = -ref(1) * ref(6) * ref(11) + 
              ref(1) * ref(7) * ref(10) + 
              ref(5) * ref(2) * ref(11) - 
              ref(5) * ref(3) * ref(10) - 
              ref(9) * ref(2) * ref(7) + 
              ref(9) * ref(3) * ref(6);

    inv[7] = ref(0) * ref(6) * ref(11) - 
             ref(0) * ref(7) * ref(10) - 
             ref(4) * ref(2) * ref(11) + 
             ref(4) * ref(3) * ref(10) + 
             ref(8) * ref(2) * ref(7) - 
             ref(8) * ref(3) * ref(6);

    inv[11] = -ref(0) * ref(5) * ref(11) + 
               ref(0) * ref(7) * ref(9) + 
               ref(4) * ref(1) * ref(11) - 
               ref(4) * ref(3) * ref(9) - 
               ref(8) * ref(1) * ref(7) + 
               ref(8) * ref(3) * ref(5);

    inv[15] = ref(0) * ref(5) * ref(10) - 
              ref(0) * ref(6) * ref(9) - 
              ref(4) * ref(1) * ref(10) + 
              ref(4) * ref(2) * ref(9) + 
              ref(8) * ref(1) * ref(6) - 
              ref(8) * ref(2) * ref(5);

    det = ref(0) * inv[0] + ref(1) * inv[4] + 
          ref(2) * inv[8] + ref(3) * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
      //invOut[i] = inv[i] * det;
      ref(i) = inv[i] * det;

    return true;
  }
};


#endif // __MINMATRIX_H_GUARD__
