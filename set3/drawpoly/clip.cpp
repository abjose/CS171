#include <vector>
#include <ostream>
#include <iostream>
#include <assert.h>

#include "draw.h"

using namespace std;

bool outof(vertex_t v, index_t d, coord_t k)
{
  if (k == -1.0)
    return v[d] < -1.0;
  else if (k == 1.0)
    return v[d] > 1.0;
  return false;
}

// Interd1mensi0nal-Interpolat0r
coord_t nm_intersect(coord_t y, edge_t edge, index_t n, index_t m)
{
  // Whaaaaaaaat are you doing?
  // You can't parameterize over the same dimesion as you are trying to generate a value for!
  assert(n != m);
  vertex_t xy0, xy1;
  xy0.push_back(edge[0][n]);
  xy0.push_back(edge[0][m]);
  xy1.push_back(edge[1][n]);
  xy1.push_back(edge[1][m]);
  // make sure this line segment increases in y
  if (xy0[1] > xy1[1])
    {
      vertex_t temp = xy0;
      xy0 = xy1;
      xy1 = temp;
    }
  coord_t dx = xy1[0] - xy0[0];
  coord_t dy = xy1[1] - xy0[1];
  if (dy == 0)
    return xy0[0]; // need to return "all" x values?
  if (dx == 0)
    return xy0[0]; // all x values same
  coord_t x = xy0[0] + (dx * (y-xy0[1])) / dy;
  return x;
}

// this clips against the normalized device coordinate
// cube (-1.0, 1.0), (-1.0, 1.0), (-1.0, 1.0)
polygon_t clipPolygon(polygon_t poly)
{
  polygon_t curpoly = poly;
  polygon_t newpoly;

  if (poly.size() < 1)
    return poly;

  // This works in three dimensions
  for (index_t d = 0; d < 3; d++)
    {
      for (coord_t k = -1.0; k < 2.0; k += 2.0)    // for k = -1.0 and then 1.0
        {
	  //cerr << "K: " << k << endl;
          newpoly.clear();
          index_t n = curpoly.size();
          for (index_t i = 0; i < n; i++)
            {
              index_t iplus1 = (i+1) % n;
              bool out_me = outof(curpoly[i], d, k);
              bool out_next = outof(curpoly[iplus1], d, k);
              bool crosses = out_me != out_next;
              if (crosses)
                {
                  edge_t e; // an edge
                  e.push_back(curpoly[i]);
                  e.push_back(curpoly[iplus1]);
                  vertex_t cpoint;
		  cpoint.clear();
                  index_t vertexsize = e[0].size();
                  for (index_t m = 0; m < vertexsize; m++)
                    {
		      //cerr << "DIMENSION " << m << endl;
                      if (d == m) // only works with whatever dimension we are not clipping on
                        {
			  //cerr << "CROSSED AT " << k << endl;
                          cpoint.push_back(k);
                          continue;
                        }
                      // m is n and d is m for nm_intersect (the Interd1mensi0nal-Interpolat0r)
		      coord_t c = nm_intersect(k, e, m, d); // a new piece of vertex, interpolated
                      cpoint.push_back(c);
                    }
                  if (out_next) // in to out
		    {
		      newpoly.push_back(cpoint);
		      //cerr << cpoint[0] << ' ' << cpoint[1] << ' ' << cpoint[2] << endl;
		    }
                  else // out to in
                    {
                      newpoly.push_back(cpoint);
		      //cerr << cpoint[0] << ' ' << cpoint[1] << ' ' << cpoint[2] << endl;
		      vertex_t cp = curpoly[iplus1];
                      newpoly.push_back(cp);
		      //cerr << cp[0] << ' ' << cp[1] << ' ' << cp[2] << endl;
                    }
                }
              else // if not crosses
                {
                  if (!out_next)  // in to in
		    {
		      vertex_t cp = curpoly[iplus1];
		      newpoly.push_back(cp);
		      //cerr << cp[0] << ' ' << cp[1] << ' ' << cp[2] << endl;
		    }
                }
            }
          curpoly = newpoly;
        }
    }

    index_t n = curpoly.size();
    for (index_t i = 0; i < n; i++)
      {
        vertex_t v = curpoly[i];
        cerr << i << " ==> " << v[0] << ' ' << v[1] << ' ' << v[2] << endl;
      }

    return curpoly;

  // polygon drawer assumes polygon xy's will be counter-clockwise
  // right hand rule facing out of the screen
  /*
#define polyFacingOut(X) true
  if (polyFacingOut(curpoly))
    {
      return curpoly;
    }
  else
    {
      newpoly.clear();
      index_t n = curpoly.size();
      for (index_t i = n-1; i >= 0; i--)
	  newpoly.push_back(curpoly[i]);
      return newpoly;
    }
  */
}
