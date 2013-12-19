#ifndef __CANVAS_H_GUARD__
#define __CANVAS_H_GUARD__

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <limits>

// really wanted to use Matrix for this, stupid templates
// TODO: should make usage of X,Y vs R,C more...better.
// TODO: add z-buff stuff
template <typename T>
class Canvas {

private:
  int R,C;
  float xMin, yMin, xMax, yMax;
  //Canvas<T> z_buff = *this; //??
  std::vector<T> matrix;

public:
  const int WHITE = 255;

  // return size
  int size() const { return R*C; }
  int nrows() const { return R; }
  int ncols() const { return C; }
  
  // directly copy elements from an array
  void copy(const std::vector<int> &arr, 
	    float xmin, float ymin,
	    float xmax, float ymax,
	    int xres, int yres) {
    matrix.clear();
    R=xres; C=yres;
    xMin = xmin; xMax = xmax;
    yMin = ymin; yMax = ymax;
    matrix = arr;
  }
  

  //------------------------------------------
  // constructors
  //------------------------------------------

  // constructors
  Canvas(float xmin, float ymin,
	 float xmax, float ymax,
	 int xres, int yres) {
    R=xres; C=yres;
    xMin = xmin; xMax = xmax;
    yMin = ymin; yMax = ymax;
    matrix = std::vector<T> (R*C);
    //z_buff.clear(std::numeric_limits<int>::max());
  }

  Canvas() {
    Canvas(0,0,0,0,0,0);
  }

  // Destructor
  ~Canvas() {}

  // Copy constructor - make deep copy
  Canvas(const Canvas& m2) { 
    copy(m2.matrix, 
	 m2.xMin, m2.yMin, 
	 m2.xMax, m2.yMax, 
	 m2.R, m2.C); 
  }

  // = operator for assignment
  Canvas &operator=(const Canvas &m2) {
    if (this != &m2)
      copy(m2.matrix, 
	 m2.xMin, m2.yMin, 
	 m2.xMax, m2.yMax, 
	 m2.R, m2.C); 
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
  const T &ref(int i) const { 
    return (*this)[i]; 
  }
  T &ref(int i) { 
    return (*this)[i]; 
  }


  //------------------------------------------
  // scaling
  //------------------------------------------

  // Helper function to convert a real x position into an integer pixel coord.
  int getPixelX(float x) {
    return (int)((x - xMin) / (xMax - xMin) * C);
  }

  // Helper function to convert a real y position into an integer pixel coord.
  int getPixelY(float y) {
    return (int)((y - yMin) / (yMax - yMin) * R);
  }


  //------------------------------------------
  // line-creation
  //------------------------------------------

  void draw_line(float x0f, float y0f, float x1f, float y1f,
		 T data, bool scale=false) {
    /* Use Bresenham's line drawing algorithm to draw a line on the canvas. 
       Pseudocode from Wikipedia. */
    int x0,y0,x1,y1;
    // we can scale if we want to 
    if (scale) {
      x0 = getPixelX(x0f); y0 = getPixelY(y0f);
      x1 = getPixelX(x1f); y1 = getPixelY(y1f);
    } else {
      x0 = (int) x0f; y0 = (int) y0f;
      x1 = (int) x1f; y1 = (int) y1f;
    }
    
    float dx = std::abs(x1-x0);
    float dy = std::abs(y1-y0);
    float sx, sy;
    if (x0 < x1) sx = 1;
    else         sx = -1;
    if (y0 < y1) sy = 1;
    else         sy = -1;
    float err = dx-dy;
    
    while(true) {
      //ref(x0,y0) = WHITE;
      if (in_bounds(C-y0,x0)) {
	ref(C-y0,x0) = data;
      }
      if (x0 == x1 && y0 == y1) break;
      float e2 = 2*err;
      if (e2 > -dy) { 
	err = err - dy;
	x0 = x0 + sx;
      } 
      if (x0 == x1 && y0 == y1) {
	//ref(x0,y0) = WHITE;
	if (in_bounds(C-y0,x0)) {
	  ref(C-y0,x0) = data;
	}
	break;
      }
      if (e2 < dx) {
	err = err + dx;
	y0 = y0 + sy;
      }
    }
  }


  //------------------------------------------
  // other drawing stuff
  //------------------------------------------

  // should move coordinate scaling/conversion stuff to its own function
  // and then maybe get rid of two versions of each function, just have an
  // extra value like scale=False that you can set to do the conversions

  void set_pixel(float xf, float yf, T data, bool scale=false) {
    int x, y;
    // we can scale if we want to 
    if (scale) {
      x = getPixelX(xf); y = getPixelY(yf);
    } else {
      x = (int) xf; y = (int) yf;
    }

    ref(x,y) = data;
  }


  void set_pixel_with_zbuff(float xf, float yf, T data, bool scale=false) {
    // set pixel, but check the z-buffer as well
    // wait, this shouldn't be here
    // HMM, maybe so you don't have to deal with a z-buffer, just use RGBA
    // and set the A value to be the Z value of the pixel? then can just check
    // that...
    // but need to make sure to default to inifinity ...

    // need to make sure that doing canvas.clear(Matrix ...) doesn't make it
    // so each element referenes the same matrix
    // should also define something in matrix (and also canvas) so can print
    // normally (like in a cout thing)
  }
  
  //------------------------------------------
  // output
  //------------------------------------------

  void display() const {
    std::cout << R << "x" << C << " matrix:" << std::endl;
    for (int r=0; r<R; r++) {
      for (int c=0; c<C; c++)
	std::cout << ref(r,c) << " ";
      std::cout << std::endl;
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Canvas &C) {
    os << C.nrows() << "x" << C.ncols() << " canvas:\n";
    for (int r=0; r<C.nrows(); r++) {
      for (int c=0; c<C.ncols(); c++)
	os << C(r,c) << " ";
      os << std::endl;
    }
    return os;
  }

  std::string to_ppm() {
    // add header
    std::string out = "P3\n";
    // add size
    out += std::to_string(R) +" "+ std::to_string(C) + "\n";
    // add max intensity
    out += std::to_string(WHITE) + "\n\n";

    // add pixel values
    for(int val: matrix) {
      std::string str = std::to_string(val);
      out += str +" "+ str +" "+ str + "\n";
    }
    
    return out;
  }
};


#endif // __CANVAS_H_GUARD__
