#include "buffer.h"

#include "SDL.h"
#include "Dense"

using namespace Eigen;

const double INF = 1e18;

class Renderer;

class Screen {
public:
    Screen(Renderer *renderer, int width, int height);

    ~Screen();

    void setPixelColor(int x, int y, Uint32 color);
    void setPixelZ(int x, int y, double z);

    void clear();
    void update();

    Vector3d getPixel(Vector4d point);

    Uint32 mixColors(Uint32 color1, Uint32 color2, double t);

    void drawPoint(int x, int y, double z, Uint32 color);
    void drawPixelLine(Vector3d point1, Vector3d point2, Uint32 color1, Uint32 color2);
    void drawTriangle(Vector3d point1, Vector3d point2, Vector3d point3, Uint32 color1, Uint32 color2, Uint32 color3);

private:
    Renderer *renderer;
    SDL_Surface *surface;
    SDL_Window *window;
    Buffer<double> zBuffer;
};