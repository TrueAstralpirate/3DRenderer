#include <vector>

#include "camera.h"
#include "screen.h"
#include "world.h"

#include "Dense"

using namespace Eigen;

class Renderer {
public:
    Renderer();

    Renderer(Camera *camera, Screen *screen, World *world);

    void getInfo();

    Vector4d getScreenPoint(Vector4d point);

    void drawPoint(Vector4d point, Uint32 pixelColor);
    void drawLine(Vector4d point1, Vector4d point2, Uint32 pixelColor1, Uint32 pixelColor2);
    void drawTriangle(Vector4d point1, Vector4d point2, Vector4d point3, Uint32 pixelColor1, Uint32 pixelColor2, Uint32 pixelColor3);

    void drawEdge(Object &obj, std::pair<int, int> edgeBorders);
    void drawObject(Object &obj);

    void update();

    void addObject(Object object);
    void addFile(const std::string& s, Vector4d pos);

    void moveFromBasis(double coef, int pos);
    void moveCamera(Vector4d move);
    void rotateCamera(double angle1, double angle2);

    void render();
    
private:
    Camera *camera;
    Screen *screen;
    World *world;
};
