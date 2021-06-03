#include <math.h>
#include <iostream>

#include "camera.h"

Camera::Camera(Vector4d position, double rotX, double rotY, double rotZ) : position(position), shortBasis(4, 2) {
    Vector4d v1 = Vector4d::UnitX();
    Vector4d v2 = Vector4d::UnitY();
    Matrix3d m;
    m = AngleAxisd(rotX / 180.0 * M_PI, Vector3d::UnitX())
    * AngleAxisd(rotY / 180.0 * M_PI,  Vector3d::UnitY())
    * AngleAxisd(rotZ / 180.0 * M_PI, Vector3d::UnitZ());
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

void Camera::move(Vector4d shift) {
    position += shift;
}

void Camera::rotateBasis(double angle1, double angle2) {
    MatrixXd m = (Matrix3d) (AngleAxisd(angle1, fullBasis.col(1).head(3)) * AngleAxisd(angle2, fullBasis.col(0).head(3)));
    m.conservativeResize(4, 4);
    m.col(3) = Vector4d(0, 0, 0, 1);
    m.row(3) = Vector4d(0, 0, 0, 1);

    fullBasis = m * fullBasis;
    inversedFullBasis = inversedFullBasis * m.transpose();
}

void Camera::normalizeCameraBasis() {
    shortBasis.col(0).normalize();
    shortBasis.col(1).normalize();
}

void Camera::createFullBasis() {
    Vector3d vector1 = shortBasis.col(0).head(3);
    Vector3d vector2 = shortBasis.col(1).head(3);
    VectorXd vector3 = vector1.cross(vector2);
    vector3.conservativeResize(4);

    fullBasis.col(0) = shortBasis.col(0);
    fullBasis.col(1) = shortBasis.col(1);
    fullBasis.col(2) = vector3;
    fullBasis.col(2).normalize();
    fullBasis.col(3) = Vector4d(0, 0, 0, 1);

    inversedFullBasis = fullBasis.inverse();
}

void Camera::updateFullBasis() {
    Vector3d vector1 = fullBasis.col(0).head(3);
    Vector3d vector2 = fullBasis.col(1).head(3);
    fullBasis.col(2).head(3) = vector1.cross(vector2);

    inversedFullBasis = fullBasis.inverse();
}

void Camera::printFullBasis() {
    std::cout << fullBasis << '\n';
}

void Camera::applyTransformToCamera(Matrix4d transform) {
    shortBasis = transform * shortBasis;
    normalizeCameraBasis();
    createFullBasis();
}

Vector4d Camera::transformPointToCameraBasis(Vector4d point) {
    point -= position;
    return inversedFullBasis * point;
}

Vector4d Camera::transformToNearPlane(Vector4d point) {
    point = transformPointToCameraBasis(point);
    if (point[2] == 0) {
        return position;
    }
    return Vector4d(-nearPlaneDistance * point[0] / point[2], -nearPlaneDistance * point[1] / point[2], -nearPlaneDistance, 1);
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

Vector4d Camera::transformToCube(Vector4d point) {
    return cubeTransform * point;
}

Vector2d Camera::projectToScreen(Vector4d point) {
    point = transformToCube(point);
    if (point[2] >= -1 && point[2] <= 1) {
        return point.head(2);
    }
    return Vector2d(-2, -2);
}

double Camera::getZ(Vector4d point) {
    point = transformToCube(point);
    if (point[2] >= -1 && point[2] <= 1) {
        return point[2];
    }
    return 2;
}

Vector3d Camera::getLightVector() {
    return fullBasis.col(2).head(3);
}

Vector4d Camera::fullProject(Vector4d point) {
    point = transformToCube(point);
    if (point[3] != 0) {
        return point;
    }
    return Vector4d(0, 0, 1000000, 1);
}

double Camera::getXAngle() {
    return fieldOfView;
}

double Camera::getYAngle() {
    return fieldOfView / displayRatio;
}
