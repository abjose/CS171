#include "raster.h"

// Static helper variables used to convert from whatever window coordinates
//  are being used into integer pixel coordinates.
static float windowXMin, windowXMax, windowYMin, windowYMax;
static int xRes, yRes;

// Must be called before any drawing occurs.  Initializes window variables for
//  proper integer pixel coordinates.
void initDraw(float xMin, float xMax, float yMin, float yMax, int xr, int yr) {
    windowXMin = xMin;
    windowXMax = xMax;
    windowYMin = yMin;
    windowYMax = yMax;
    xRes = xr;
    yRes = yr;
}

// Helper function to convert a real x position into an integer pixel coord.
static int getPixelX(float x) {
    return (int)((x - windowXMin) / (windowXMax - windowXMin) * xRes);
}

// Helper function to convert a real y position into an integer pixel coord.
static int getPixelY(float y) {
    return (int)((y - windowYMin) / (windowYMax - windowYMin) * yRes);
}

// Limited use helper struct.
struct rasterCoord {
    int x, y;
};

// Helper function f as defined in the course text.  Not sure exactly what
//  it means, on a conceptual level.
static int f(rasterCoord vert0, rasterCoord vert1, int x, int y) {
    int x0 = vert0.x;
    int y0 = vert0.y;
    int x1 = vert1.x;
    int y1 = vert1.y;
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

// The meat of the package.  Takes three vertices in arbitrary order, with each
//  vertex consisting of an x and y value in the first two data positions, and
//  any arbitrary amount of extra data, and calls the passed in function on
//  every resulting pixel, with all data values magically interpolated.
void raster(vertex verts[3], void (*drawPixel)(int, int, float *)) {
    // Pixel coordinates for a bounding box
    int xMin, xMax, yMin, yMax;
    // High enough that it doesn't matter
    xMin = xRes + 1; yMin = yRes + 1;
    // Low enough that it doesn't matter
    xMax = yMax = -1;
    
    // Storage space to convert each vertex's x and y to pixel coordinates
    rasterCoord *coords = new rasterCoord[3];
    
    // do the conversion
    for (int i = 0; i < 3; i++) {
        coords[i].x = getPixelX(verts[i].data[0]);
        coords[i].y = getPixelY(verts[i].data[1]);
    }
    
    // find the bounding box
    for (int i = 0; i < 3; i++) {
        if (coords[i].x < xMin)
            xMin = coords[i].x;
        if (coords[i].y < yMin)
            yMin = coords[i].y;
        if (coords[i].x > xMax)
            xMax = coords[i].x;
        if (coords[i].y > yMax)
            yMax = coords[i].y;
    }
    
    // normalizing values for the barycentric coordinates
    float fAlpha, fBeta, fGamma;
    
    // not sure exactly what's going on here, so read the textbook
    fAlpha = f(coords[1], coords[2], coords[0].x, coords[0].y);
    fBeta = f(coords[2], coords[0], coords[1].x, coords[1].y);
    fGamma = f(coords[0], coords[1], coords[2].x, coords[2].y);
    
	// check for zero denominators. if found, these indicate a degenerate
	// triangle which should not be drawn, so just return.
	if(fabs(fAlpha) < .0001 || fabs(fBeta) < .0001 || fabs(fGamma) < .0001)
		return;
	
    // go over every pixel in the bounding box
    for (int y = (int)((yMin > 0) ? yMin : 0); y < yMax && y < yRes; y++) {
        for (int x = (int)((xMin > 0) ? xMin : 0); x < xMax && x < xRes; x++) {
            
            // calculate the pixel's barycentric coordinates
            float alpha = f(coords[1], coords[2], x, y) / fAlpha;
            float beta = f(coords[2], coords[0], x, y) / fBeta;
            float gamma = f(coords[0], coords[1], x, y) / fGamma;
            
            // if the coordinates are positive, do the next check
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                    // create storage to send interpolated data
                    int numData = verts[0].numData;
                    float *data = new float[numData];
                    
                    // interpolate all data
                    for (int i = 0; i < numData; i++) {
                        data[i] = (alpha * verts[0].data[i] +
                                   beta * verts[1].data[i] +
                                   gamma * verts[2].data[i]);
                    }
                    
                    // and finally, draw the pixel
                    drawPixel(x, y, data);
            }
            
        }
    }
}
