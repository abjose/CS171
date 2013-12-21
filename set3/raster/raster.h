#ifndef __RASTER_H__
#define __RASTER_H__

#include <math.h>

// Vertex structure for the passing of data.  Necessary conditions for use are that
//  a) the size of data is equal to numData
//  b) the x and y coordinates in whatever window space you choose to use are stored
//     in data[0] and data[1]
//  Aside from that, feel free to pass as much or as little data as you want to be
//  interpolated.
struct vertex {
    float *data;
    int numData;
};

// Must be called before any drawing occurs.  Initializes window variables for
//  proper integer pixel coordinates.  Min and Max correspond to the real-valued
//  bounding box of your window space.  xr and yr are the resolution of the window
//  in the x and y directions
void initDraw(float xMin, float xMax, float yMin, float yMax, int xr, int yr);

// The meat of the package.  Takes three vertices in arbitrary order, with each
//  vertex consisting of an x and y value in the first two data positions, and
//  any arbitrary amount of extra data, and calls the passed in function on
//  every resulting pixel, with all data values magically interpolated.
//  The function, drawPixel, should have the form
//   drawPixel(int x, int y, float *data)
//  where x and y are pixel coordinates, and data is the interpolated data as
//  defined by the vertices.
void raster(vertex verts[3], void (*drawPixel)(int, int, float *));

#endif // __RASTER_H__
