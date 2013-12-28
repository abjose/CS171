#include <assert.h>
#include "draw.h"

static coord_t xmin, xmax, ymin, ymax;
static index_t xres, yres;

void initDraw(coord_t _xmin, coord_t _xmax, coord_t _ymin, coord_t _ymax,
	      index_t _xres, index_t _yres)
{
   xmin = _xmin;
   xmax = _xmax;
   ymin = _ymin;
   ymax = _ymax;
   xres = _xres;
   yres = _yres;

   assert(xmax > xmin);
   assert(ymax > ymin);
   assert(xres > 0);
   assert(yres > 0);
}

// Convert an x coordinate into the corresponding pixel coordinate.
index_t getX(coord_t x) {
   return (index_t)((x-xmin)/(xmax-xmin)*xres);
}

// Convert a y coordinate into the corresponding pixel coordinate.
// We mirror the image vertically because Cartesian coordinates have
// the origin in the lower-left, while PPM images have it in the
// upper-left.
index_t getY(coord_t y) {
   return (index_t)(yres - ((y-ymin)/(ymax-ymin)*yres));
}

coord_t smallest(vertex_t seq)
{
  bool changed = false;
  coord_t a;
  coord_t len = seq.size();
  for (index_t i = 0; i < len; i++)
    {
      coord_t e = seq[i];
      if (!changed)
        {
          a = e;
          changed = true;
        }
      else if (e < a)
          a = e;
    }
  return a;
}

coord_t greatest(vertex_t seq)
{
  bool changed = false;
  coord_t a;
  index_t len = seq.size();
  for (index_t i = 0; i < len; i++)
    {
      coord_t e = seq[i];
      if (!changed)
        {
          a = e;
          changed = true;
        }
      else if (e > a)
          a = e;
    }
  return a;
}

indices_t edgesof(coord_t y, edges_t edges)
{
  indices_t ret;
  index_t len = edges.size();
  for (index_t i = 0; i < len; i++)
    {
      edge_t e = edges[i];
      vertex_t ys;
      ys.push_back(e[0][1]);
      ys.push_back(e[1][1]);
      if (y >= smallest(ys) && y <= greatest(ys))
        {
          ret.push_back(i);
        }
    }
  return ret;
}

coord_t n_intersect(coord_t y, edge_t edge, index_t n)
{
  return nm_intersect(y, edge, n, 1);
}

coord_t xintersect(coord_t y, edge_t edge)
{
  return n_intersect(y, edge, 0);
}

edgeindex_x_t leftej(edgeindex_x_s_t exs)
{
  index_t a = xres+10;
  edgeindex_x_t ret;
  index_t len = exs.size();
  for (int i = 0; i < len; i++)
    {
      edgeindex_x_t ex = exs[i];
      if (ex.x < a) // if ex[X] < a
        {
          ret = ex;
          a = ex.x;
        }
    }
  return ret;
}

edgeindex_x_t rightej(edgeindex_x_s_t exs)
{
  index_t a = -10;
  edgeindex_x_t ret;
  index_t len = exs.size();
  for (int i = 0; i < len; i++)
    {
      edgeindex_x_t ex = exs[i];
      if (ex.x > a) // if ex[X] < a
        {
          ret = ex;
          a = ex.x;
        }
    }
  return ret;
}

// the polygon is a vector of vertex_t's
// a vertex is a vector of coord_t's (floats or doubles)
// looking like this:
//   rx, ry, z[, a, b, c, ...]
//   you must have rx and ry, which are the x,y in raster (pixel) coordinates
//   to be useful, you probably need a z and then you can tack anything
//   on the end. These values get automagically interpolated as well
//   so you can do things like put the actual vertex (untransformed) coords
//   and the normals and/or the colors or whatever your drawShadeFunc needs
//   to find the right color to draw a pixel!
void drawPolygon(polygon_t poly, func_t drawShadeFunc)
{
  index_t n = poly.size();
  if (!n)
    return;
  index_t dims = poly[0].size();

  edges_t edges;
  edge_t edge;
  for (index_t i = 0; i < n; i++)
    {
      edge.clear();
      edge.push_back(poly[i]);
      edge.push_back(poly[(i+1) % n]);
      edges.push_back(edge);
    }
  vertex_t ys;
  for (index_t i = 0; i < n; i++)
    ys.push_back((index_t)poly[i][1]);
  index_t ymi = (index_t)smallest(ys);
  index_t yma = (index_t)greatest(ys);

  edgeindices_x_t one_yej;
  edgeindices_x_s_t yej;
  for (index_t y = ymi; y < yma; y++)
    {
      one_yej.x = y;
      one_yej.eis = edgesof(y, edges);
      yej.push_back(one_yej);
    }

  edgeindex_x_s_t exs;
  edge_t e;
  indices_t ejs;
  index_t num_ys = yej.size();
  if (num_ys < 1)
    return;
  index_t y0 = yej[0].x;
  for (index_t y = y0; y < y0 + num_ys; y++)
    {
      ejs = yej[y-y0].eis;
      if (y < 0 || y >= yres)
        continue;

      exs.clear();
      //xs.clear();
      index_t e, x;
      coord_t ejs_len = ejs.size();
      edgeindex_x_t ex;
      for (index_t i = 0; i < ejs_len; i++)
        {
          e = ejs[i];
          x = (index_t)xintersect((coord_t)y, edges[e]);
          ex.ei = e;
          ex.x = x;
          exs.push_back(ex);
        }
      edgeindex_x_t le = leftej(exs);
      edgeindex_x_t re = rightej(exs);

      edge_t lej = edges[le.ei];
      edge_t rej = edges[re.ei];

      index_t x0 = le.x;
      index_t x1 = re.x;
      index_t dx = x1 - x0 + 1;

      vertex_t zplus;
      vertex_t dzplus;
      for (index_t i = 2; i < dims; i++)
        {
          coord_t scalar0 = n_intersect((coord_t)y, lej, i);
          coord_t scalar1 = n_intersect((coord_t)y, rej, i);
          coord_t dz = (scalar1 - scalar0) / ((coord_t)dx);
          zplus.push_back(scalar0);
          dzplus.push_back(dz);
        }

      // now actually call drawShadeFunc on every x in the scanline
      for (index_t x = x0; x < x1; x++)
        {
          vertex_t vert;
          vert.push_back((coord_t)x);
          vert.push_back((coord_t)y);
          for (index_t i = 0; i < zplus.size(); i++)
            {
              vert.push_back(zplus[i]);
              zplus[i] += dzplus[i];
            }
          drawShadeFunc(vert);
        }
    }
}
