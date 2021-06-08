#include <math.h>
#include <iostream>

#include "camera.h"

namespace Project {

Camera::Camera(Eigen::Vector4d position, double rotX, double rotY, double rotZ) : position(position), shortBasis(4, 2) {
    Eigen::Vector4d v1 = Eigen::Vector4d::UnitX();
    Eigen::Vector4d v2 = Eigen::Vector4d::UnitY();
    Eigen::Matrix3d m;
    m = Eigen::AngleAxisd(rotX / 180.0 * M_PI, Eigen::Vector3d::UnitX())
    * Eigen::AngleAxisd(rotY / 180.0 * M_PI,  Eigen::Vector3d::UnitY())
    * Eigen::AngleAxisd(rotZ / 180.0 * M_PI, Eigen::Vector3d::UnitZ());
    v1.head(3) = m * v1.head(3);
    v2.head(3) = m * v2.head(3);
    shortBasis.col(0) = v1;
    shortBasis.col(1) = v2;
    normalizeCameraBasis();
    createFullBasis();
    updateCubeTransform(-1, 1, -1, 1);
};

void Camera::moveFromBasis(double coef, int pos) {
    position += fullBasis.col(pos) * coef;
}

void Camera::move(Eigen::Vector4d shift) {
    position += shift;
}

void Camera::rotateBasis(double rotateXAngle, double rotateYAngle) {
    Eigen::MatrixXd m = (Eigen::Matrix3d) (Eigen::AngleAxisd(rotateXAngle, fullBasis.col(1).head(3)) * Eigen::AngleAxisd(rotateYAngle, fullBasis.col(0).head(3)));
    m.conservativeResize(4, 4);
    m.col(3) = Eigen::Vector4d(0, 0, 0, 1);
    m.row(3) = Eigen::Vector4d(0, 0, 0, 1);

    fullBasis = m * fullBasis;
    inversedFullBasis = inversedFullBasis * m.transpose();
}

void Camera::normalizeCameraBasis() {
    shortBasis.col(0).normalize();
    shortBasis.col(1).normalize();
}

void Camera::createFullBasis() {
    Eigen::Vector3d vector1 = shortBasis.col(0).head(3);
    Eigen::Vector3d vector2 = shortBasis.col(1).head(3);
    Eigen::VectorXd vector3 = vector1.cross(vector2);
    vector3.conservativeResize(4);

    fullBasis.col(0) = shortBasis.col(0);
    fullBasis.col(1) = shortBasis.col(1);
    fullBasis.col(2) = vector3;
    fullBasis.col(2).normalize();
    fullBasis.col(3) = Eigen::Vector4d(0, 0, 0, 1);

    inversedFullBasis = fullBasis.inverse();
}

void Camera::updateFullBasis() {
    Eigen::Vector3d vector1 = fullBasis.col(0).head(3);
    Eigen::Vector3d vector2 = fullBasis.col(1).head(3);
    fullBasis.col(2).head(3) = vector1.cross(vector2);

    inversedFullBasis = fullBasis.inverse();
}

void Camera::printFullBasis() {
    std::cout << fullBasis << '\n';
}

void Camera::applyTransformToCamera(Eigen::Matrix4d transform) {
    shortBasis = transform * shortBasis;
    normalizeCameraBasis();
    createFullBasis();
}

Eigen::Vector4d Camera::transformPointToCameraBasis(Eigen::Vector4d point) {
    point -= position;
    return inversedFullBasis * point;
}

Eigen::Vector4d Camera::transformToNearPlane(Eigen::Vector4d point) {
    point = transformPointToCameraBasis(point);
    if (point[2] == 0) {
        return position;
    }
    return Eigen::Vector4d(-nearPlaneDistance * point[0] / point[2], -nearPlaneDistance * point[1] / point[2], -nearPlaneDistance, 1);
}

void Camera::updateCubeTransform(double l, double r, double b, double t) {
    cubeTransform(0, 0) = 2 * nearPlaneDistance / (r - l);
    cubeTransform(0, 2) = (r + l) / (r - l);
    cubeTransform(1, 1) = (2 * nearPlaneDistance) / (t - b);
    cubeTransform(1, 2) = (t + b) / (t - b);
    cubeTransform(2, 2) = -(farPlaneDistance + nearPlaneDistance) / (farPlaneDistance - nearPlaneDistance);
    cubeTransform(2, 3) = -2.0 * nearPlaneDistance * farPlaneDistance / (farPlaneDistance - nearPlaneDistance);
    cubeTransform(3, 2) = -1;
}

void Camera::printCubeTransform() {
    std::cout << cubeTransform << '\n';
}

Eigen::Vector4d Camera::transformToCube(Eigen::Vector4d point) {
    return cubeTransform * point;
}

Eigen::Vector2d Camera::projectToScreen(Eigen::Vector4d point) {
    point = transformToCube(point);
    if (point[2] >= -1 && point[2] <= 1) {
        return point.head(2);
    }
    return Eigen::Vector2d(-2, -2);
}

double Camera::getZ(Eigen::Vector4d point) {
    point = transformToCube(point);
    if (point[2] >= -1 && point[2] <= 1) {
        return point[2];
    }
    return 2;
}

Eigen::Vector3d Camera::getLightVector() {
    return fullBasis.col(2).head(3);
}

Eigen::Vector4d Camera::fullProject(Eigen::Vector4d point) {
    point = transformToCube(point);
    if (point[3] != 0) {
        return point;
    }
    return Eigen::Vector4d(0, 0, 1000000, 1);
}

double Camera::getXAngle() {
    return fieldOfView;
}

double Camera::getYAngle() {
    return fieldOfView / displayRatio;
}

}
