#include <math.h>
#include <iostream>

#include "camera.h"

Camera::Camera(Vector4d position, Vector4d v1, Vector4d v2) : position(position), shortBasis(4, 2) {
    shortBasis.col(0) = v1;
    shortBasis.col(1) = v2;
    normalizeCameraBasis();
    updateCubeTransform(-1, 1, -1, 1);
};

void Camera::moveFromBasis(const double coef, const int pos) {
    position += fullBasis.col(pos) * coef;
}

void Camera::move(Vector4d shift) {
    position += shift;
}

void Camera::rotateX(const double angle) {
    Vector3d v = shortBasis.col(0).head(3);
    Vector3d axis = shortBasis.col(1).head(3);
    double ca = cos(angle);
    double sa = sin(angle);
    VectorXd res = v * ca + axis.cross(v) * sa + axis * axis.dot(v) * (1 - ca);
    res.conservativeResize(4);
    shortBasis.col(0) = res;
}

void Camera::rotateY(const double angle) {
    Vector3d v = shortBasis.col(1).head(3);
    Vector3d axis = shortBasis.col(0).head(3);
    double ca = cos(angle);
    double sa = sin(angle);
    VectorXd res = v * ca + axis.cross(v) * sa + axis * axis.dot(v) * (1 - ca);
    res.conservativeResize(4);
    shortBasis.col(1) = res;
}

void Camera::rotate(const Vector3d axis, const double angle) {
    Vector3d v0 = shortBasis.col(0).head(3);
    Vector3d v1 = shortBasis.col(1).head(3);
    double ca = cos(angle);
    double sa = sin(angle);
    VectorXd res0 = v0 * ca + axis.cross(v0) * sa + axis * axis.dot(v0) * (1 - ca);
    res0.conservativeResize(4);
    shortBasis.col(0) = res0;
    VectorXd res1 = v1 * ca + axis.cross(v1) * sa + axis * axis.dot(v1) * (1 - ca);
    res1.conservativeResize(4);
    shortBasis.col(1) = res1;
    normalizeCameraBasis();
}

void Camera::rotateBasis(const double angle1, const double angle2) {
    rotateX(angle1);
    rotateY(angle2);
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

void Camera::printFullBasis() {
    std::cout << fullBasis << '\n';
}

void Camera::applyTransformToCamera(const Matrix4d transform) {
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
    return defaultPixel;
}

double Camera::getZ(Vector4d point) {
    point = transformToCube(point);
    if (point[2] >= -1 && point[2] <= 1) {
        return point[2];
    }
    return 2;
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