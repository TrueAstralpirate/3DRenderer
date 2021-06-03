#include <math.h>

#include "Dense"

using namespace Eigen;

class Camera {
public:
    Camera();

    Camera(Vector4d position, double rotX, double rotY, double rotZ);

    void move(Vector4d shift);
    void moveFromBasis(double coef, int pos);
    void rotateBasis(double angle1, double angle2);

    void normalizeCameraBasis();
    void createFullBasis();
    void updateFullBasis();
    void printFullBasis();
    void applyTransformToCamera(Matrix4d transform);

    Vector4d transformPointToCameraBasis(Vector4d point);
    Vector4d transformToNearPlane(Vector4d point);

    void updateCubeTransform(double l, double r, double b, double t);
    void printCubeTransform();
    Vector4d transformToCube(Vector4d point);
    Vector2d projectToScreen(Vector4d point);

    double getZ(Vector4d point);
    double getXAngle();
    double getYAngle();

    Vector3d getLightVector();

    Vector4d fullProject(Vector4d point);

private:
    static constexpr double fieldOfView = M_PI / 4.0;
    static constexpr double nearPlaneDistance = 1.0;
    static constexpr double farPlaneDistance = 100.0;
    static constexpr double displayRatio = 16.0 / 10.0; // Display ratio
    static constexpr double screenWidth = 1.0;
    static constexpr double screenHeight = screenWidth / displayRatio;

    MatrixXd shortBasis;
    Matrix4d fullBasis;
    Matrix4d inversedFullBasis;

    Vector4d position;

    Matrix4d cubeTransform;
};
