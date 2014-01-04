#ifndef __CANVAS_H_GUARD__
#define __CANVAS_H_GUARD__

#include <cassert>
#include <string>
#include <vector>

// really wanted to use Matrix for this, stupid templates
class Canvas {

private:
  int R,C;
  float xMin, yMin, xMax, yMax;
  std::vector<int> matrix;

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
    //std::cout << "got 1!!\n";
    matrix.clear();
    R=xres; C=yres;
    //std::cout << "got 2!!\n";
    xMin = xmin; xMax = xmax;
    yMin = ymin; yMax = ymax;
    matrix = arr;
    //std::cout << "got 3!!\n";
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
    matrix = std::vector<int> (R*C);
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
  void clear(int val) { 
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
  const int &operator()(int r, int c) const { 
    return matrix[r*C+c]; 
  }
  int &operator()(int r, int c) { 
    return matrix[r*C+c]; 
  }
  // non-checked accessor
  const int &ref(int row, int col) const { return matrix[col + row*C]; }
  // non-checked LValue setter
  int &ref(int row, int col) { return matrix[col + row*C]; }

  // flat addressing
  const int &operator[](int i) const { 
    assert(in_bounds(i)); 
    return matrix[i]; 
  }
  int &operator[](int i) { 
    assert(in_bounds(i)); 
    return matrix[i]; 
  }
  // to make accessing nicer here
  const int &ref(int i) const { 
    return (*this)[i]; 
  }
  int &ref(int i) { 
    return (*this)[i]; 
  }


  //------------------------------------------
  // line-creation
  //------------------------------------------

  void draw_line(int x0, int y0, int x1, int y1) {
    /* Use Bresenham's line drawing algorithm to draw a line on the canvas. 
       Pseudocode from Wikipedia. */
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
      if (in_bounds(R-y0,x0))
	ref(R-y0,x0) = WHITE;
      if (x0 == x1 && y0 == y1) break;
      float e2 = 2*err;
      if (e2 > -dy) { 
	err = err - dy;
	x0 = x0 + sx;
      } 
      if (x0 == x1 && y0 == y1) {
	//ref(x0,y0) = WHITE;
	// IF WANT TO FLIP VERT, JUST DO 1-y0??
	if (in_bounds(R-y0,x0))
	  ref(R-y0,x0) = WHITE;
	break;
      }
      if (e2 < dx) {
	err = err + dx;
	y0 = y0 + sy;
      }
    }
  }

  void scale_draw_line(float x1, float y1, float x2, float y2) {
    float xrange = xMax-xMin;
    float yrange = yMax-yMin;
    //std::cout << xMin << ", " << xMax << std::endl;
    //std::cout << yMin << ", " << yMax << std::endl;
    //std::cout << xrange << ", " << yrange << std::endl;
    // find scaled versions
    int x1_s = ((x1-xMin)/xrange) * C;
    int y1_s = ((y1-yMin)/yrange) * R;
    int x2_s = ((x2-xMin)/xrange) * C;
    int y2_s = ((y2-yMin)/yrange) * R;
    //std::cout << x1_s << ", " << y1_s << std::endl;
    //std::cout << x2_s << ", " << y2_s << std::endl;
    // draw line
    draw_line(x1_s, y1_s, x2_s, y2_s);
  }


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
