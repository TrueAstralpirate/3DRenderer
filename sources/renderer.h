#include "camera.h"
#include "screen.h"

#include "Dense"

using namespace Eigen;

class Renderer {
public:
    Renderer(Camera camera, int width, int height);

    void getInfo();

    Vector4d getScreenPoint(Vector4d point);

    void drawPoint(Vector4d point, Uint32 pixelColor);
    void drawLine(Vector4d point1, Vector4d point2, Uint32 pixelColor1, Uint32 pixelColor2);
    void drawTriangle(Vector4d point1, Vector4d point2, Vector4d point3, Uint32 pixelColor1, Uint32 pixelColor2, Uint32 pixelColor3);

    void updateCamera();
    void update();

    
private:
    Camera camera;
    Screen screen;
};