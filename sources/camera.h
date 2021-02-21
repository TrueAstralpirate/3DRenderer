#include <math.h>

#include "Dense"

using namespace Eigen;

const double pi = acos(-1);

class Camera {
public:

    Camera(Vector4d position, Vector4d v1, Vector4d v2);

    void normalizeCameraBasis();
    void createFullBasis();
    void printFullBasis();
    void applyTransformToCamera(const Matrix4d transform);

    Vector4d transformPointToCameraBasis(Vector4d point);
    Vector4d transformToNearPlane(Vector4d point);

    void updateCubeTransform(double l, double r, double b, double t);
    void printCubeTransform();
    Vector4d transformToCube(Vector4d point);
    Vector2d projectToScreen(Vector4d point);

    double getZ(Vector4d point);

private:
    const Vector2d defaultPixel = Vector2d(-2, -2);

    const double nearPlaneDistance = 1.0;
    const double fieldOfView = pi / 4.0;
    const double farPlaneDistance = 10.0;
    const double displayRatio = 16.0 / 10.0; // Display ratio
    const double screenWidth = 1.0;
    const double screenHeight = screenWidth / displayRatio;

    MatrixXd shortBasis;
    Matrix4d fullBasis;

    Vector4d position = Vector4d(0, 0, 0, 0);

    Matrix4d cubeTransform;
};