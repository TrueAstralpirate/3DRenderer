#include <vector>

#include "camera.h"
#include "screen.h"
#include "world.h"

#include "Dense"

namespace Project {

class Renderer {
public:
    Renderer();

    Renderer(Camera *camera, Screen *screen, World *world);

    void getInfo();

    Eigen::Vector4d getScreenPoint(Eigen::Vector4d point);

    void drawPoint(Eigen::Vector4d point, Uint32 pixelColor);
    void drawLine(Eigen::Vector4d point1, Eigen::Vector4d point2, Uint32 pixelColor1, Uint32 pixelColor2);
    void drawTriangle(Eigen::Vector4d point1, Eigen::Vector4d point2, Eigen::Vector4d point3, Uint32 pixelColor1, Uint32 pixelColor2, Uint32 pixelColor3);

    void drawEdge(Object &obj, std::pair<int, int> edgeBorders);
    void drawObject(Object &obj);

    void update();

    void addObject(Object object);
    void addFile(const std::string& s, Eigen::Vector4d pos);

    void moveFromBasis(double coef, int pos);
    void moveCamera(Eigen::Vector4d move);
    void rotateCamera(double angle1, double angle2);

    void render();
    
private:
    Camera *camera;
    Screen *screen;
    World *world;
};

}
