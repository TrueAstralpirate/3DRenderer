#include <math.h>

#include "Dense"

namespace Project {

class Camera {
public:
    Camera();

    Camera(Eigen::Vector4d position, double rotX, double rotY, double rotZ);

    void move(Eigen::Vector4d shift);
    void moveFromBasis(double coef, int pos);
    void rotateBasis(double rotateXAngle, double rotateYAngle);

    void normalizeCameraBasis();
    void createFullBasis();
    void updateFullBasis();
    void printFullBasis();
    void applyTransformToCamera(Eigen::Matrix4d transform);

    Eigen::Vector4d transformPointToCameraBasis(Eigen::Vector4d point);
    Eigen::Vector4d transformToNearPlane(Eigen::Vector4d point);

    void updateCubeTransform(double l, double r, double b, double t);
    void printCubeTransform();
    Eigen::Vector4d transformToCube(Eigen::Vector4d point);
    Eigen::Vector2d projectToScreen(Eigen::Vector4d point);

    double getZ(Eigen::Vector4d point);
    double getXAngle();
    double getYAngle();

    Eigen::Vector3d getLightVector();

    Eigen::Vector4d fullProject(Eigen::Vector4d point);

private:
    static constexpr double fieldOfView = M_PI / 4.0;
    static constexpr double nearPlaneDistance = 1.0;
    static constexpr double farPlaneDistance = 100.0;
    static constexpr double displayRatio = 16.0 / 10.0; // Display ratio
    static constexpr double screenWidth = 1.0;
    static constexpr double screenHeight = screenWidth / displayRatio;

    Eigen::MatrixXd shortBasis;
    Eigen::Matrix4d fullBasis;
    Eigen::Matrix4d inversedFullBasis;

    Eigen::Vector4d position;

    Eigen::Matrix4d cubeTransform;
};

}
