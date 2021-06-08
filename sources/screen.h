#include "buffer.h"

#include "SDL.h"
#include "Dense"

namespace Project {

const double INF = 1e18;

class Renderer;

class Screen {
public:
    Screen();

    Screen(int width, int height);

    ~Screen();

    void setPixelColor(int x, int y, Uint32 color);
    void setPixelZ(int x, int y, double z);

    void clear();
    void userView();

    int getWidth();
    int getHeight();

    Eigen::Vector3d getPixel(Eigen::Vector4d point);

    Uint32 intenseColor(Uint32 color, double intensitivity);
    Uint32 mixColors(Uint32& color1, Uint32& color2, Uint32& color3, const double& a, const double& b, double& c);

    double area(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2);

    void drawPoint(const int& x, const int& y, const double& z, const Uint32& color);
    void drawPixelLine(Eigen::Vector3d point1, Eigen::Vector3d point2, Uint32 color1, Uint32 color2);
    void drawHorizontalLine(Eigen::Vector3d& point1, Eigen::Vector3d& point2, Eigen::Vector3d& point3, Uint32& color1, Uint32& color2, Uint32& color3, double& triangle_area, int left, int right, int y);
    void drawTriangle(Eigen::Vector3d& point1, Eigen::Vector3d& point2, Eigen::Vector3d& point3, Uint32& color1, Uint32& color2, Uint32& color3, double intensity);

private:
    int width;
    int height;
    int run;

    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<SDL_Surface> surface;
    Buffer<int> cnt;
    Buffer<double> zBuffer;

    void sortPoints(Eigen::Vector3d& point1, Eigen::Vector3d& point2, Eigen::Vector3d& point3, Uint32& color1, Uint32& color2, Uint32& color3);
};

}
