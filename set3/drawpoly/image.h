// A packed color in 0x00BBGGRR format
typedef uint32_t pcolor;
//
// Helper macros for dealing with colors.
#define RGB(r, g, b) ((b)<<16 | (g)<<8 | (r))
#define GRAY(g) RGB(g,g,g)
#define RED(c) ((c) & 0xFF)
#define GREEN(c) (((c) & 0xFF00) >> 8)
#define BLUE(c) (((c) & 0xFF0000) >> 16)

// A 24-bit color canvas that can write to ostreams in PPM format.
// A double is used for depth.
class PPM {
protected:
   pcolor *data;
   double *depth;

public:
   const int width, height;

   PPM(const int width, const int height);
   ~PPM();

   pcolor &getPixel(int x, int y);
   double &getDepth(int x, int y);
   void setPixel(int x, int y, double z, pcolor c);
   void write(std::ostream &os);
};
