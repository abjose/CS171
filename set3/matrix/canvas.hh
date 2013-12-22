#ifndef __CANVAS_H_GUARD__
#define __CANVAS_H_GUARD__

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <limits>

// TODO: should make usage of X,Y vs R,C more...better.
// TODO: add z-buff stuff
//template <typename T>
class Canvas {

private:
  typedef Matrix<float,3,1> pixel;
  int R,C;
  float xMin, yMin, xMax, yMax;
  std::vector<pixel> matrix;

public:
  // return size
  int size() const { return R*C; }
  int nrows() const { return R; }
  int ncols() const { return C; }
  
  // directly copy elements from an array
  void copy(const std::vector<pixel> &arr, 
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
    matrix = std::vector<pixel> (R*C);
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
  void clear(pixel val) { 
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
  const pixel &operator()(int r, int c) const { 
    return matrix[r*C+c]; 
  }
  pixel &operator()(int r, int c) { 
    return matrix[r*C+c]; 
  }
  // non-checked accessor
  const pixel &ref(int row, int col) const { return matrix[col + row*C]; }
  // non-checked LValue setter
  pixel &ref(int row, int col) { return matrix[col + row*C]; }

  // flat addressing
  const pixel &operator[](int i) const { 
    assert(in_bounds(i)); 
    return matrix[i]; 
  }
  pixel &operator[](int i) { 
    assert(in_bounds(i)); 
    return matrix[i]; 
  }
  // to make accessing nicer here
  const pixel &ref(int i) const { 
    return (*this)[i]; 
  }
  pixel &ref(int i) { 
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
  // other drawing stuff
  //------------------------------------------

  // should move coordinate scaling/conversion stuff to its own function
  // and then maybe get rid of two versions of each function, just have an
  // extra value like scale=False that you can set to do the conversions

  void set_pixel(float xf, float yf, pixel data, bool scale=false) {
    int x, y;
    // we can scale if we want to 
    if (scale) {
      x = getPixelX(xf); y = getPixelY(yf);
    } else {
      x = (int) xf; y = (int) yf;
    }

    ref(x,y) = data;
  }


  void set_pixel_with_zbuff(float xf, float yf, pixel data, bool scale=false) {
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

    // could make another version of Canvas that uses this? and inherits from
    // Canvas...like Zanvas or something.

    // or could just pass a reference to a z-buffer from sceneblock or something
    // that would probably be easiest
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
    out += "255\n\n";
    //out += "1\n\n";

    // add pixel values
    for(auto& val: matrix)
      out += std::to_string((int) (255*val[0])) +" "+ 
	std::to_string((int) (255*val[1])) +" "+ 
	std::to_string((int) (255*val[2])) + "\n";
    
    return out;
  }
};


#endif // __CANVAS_H_GUARD__
