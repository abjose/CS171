#include <vector>

using namespace std;

typedef double coord_t;
typedef int index_t;

typedef vector<coord_t> vertex_t;
typedef vector<index_t> indices_t;
typedef vector<vertex_t> polygon_t;

typedef vector<vertex_t> edge_t;
typedef vector<edge_t> edges_t;

typedef struct edgex {
  edge_t e;
  index_t x;
} edge_x_t;

typedef vector<edge_x_t> edge_x_s_t;

typedef struct edgeindices_x {
  indices_t eis;
  index_t x;
} edgeindices_x_t;

typedef vector<edgeindices_x_t> edgeindices_x_s_t;

typedef struct edgeindex_x {
  index_t ei;
  index_t x;
} edgeindex_x_t;

typedef vector<edgeindex_x_t> edgeindex_x_s_t;

// the type for a function that takes a vertex_t and returns nothing
typedef void (*func_t)(vertex_t);

void initDraw(coord_t xmin, coord_t xmax, coord_t ymin, coord_t ymax, index_t xres, index_t yres);
void drawPolygon(polygon_t poly, func_t drawShadeFunc);
polygon_t clipPolygon(polygon_t poly);

index_t getX(coord_t x);
index_t getY(coord_t y);

// Interd1mensi0nal-Interpolat0r
coord_t nm_intersect(coord_t y, polygon_t edge, index_t n, index_t m);
