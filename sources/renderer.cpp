#include "renderer.h"
#include "SDL.h"

#include <iostream>

const Vector4d EPS = Vector4d(0, 0, -0.2, 0);

Renderer::Renderer(Camera *camera, Screen *screen, World *world) : camera(camera), screen(screen), world(world) {
    update();
};

void Renderer::getInfo() {
    camera->printFullBasis();
}

Vector4d Renderer::getScreenPoint(Vector4d point) {
    return camera->transformToCube(point);
}

void Renderer::drawPoint(Vector4d point, Uint32 pixelColor) {
    if (point[3] == 0) {
        return;
    }
    Vector3d info = screen->getPixel(point);
    screen->drawPoint(info[0], info[1], info[2], pixelColor);
}

void Renderer::drawLine(Vector4d point1, Vector4d point2, Uint32 pixelColor1, Uint32 pixelColor2) {
    Vector3d info1 = screen->getPixel(point1);
    Vector3d info2 = screen->getPixel(point2);
    screen->drawPixelLine(info1, info2, pixelColor1, pixelColor2);
}

void Renderer::drawTriangle(Vector4d point1, Vector4d point2, Vector4d point3, Uint32 pixelColor1, Uint32 pixelColor2, Uint32 pixelColor3) {
    Vector3d light = camera->getLightVector();
    Vector3d v1 = point2.head(3) - point1.head(3);
    Vector3d v2 = point3.head(3) - point1.head(3);
    Vector3d norm = v2.cross(v1);
    norm.normalize();
    double intensity = light.dot(norm);
    if (intensity <= 0) {
        return;
    }

    point1 = camera->transformPointToCameraBasis(point1);
    point2 = camera->transformPointToCameraBasis(point2);
    point3 = camera->transformPointToCameraBasis(point3);

    if (point1[2] > point2[2]) {
        std::swap(point1, point2);
        std::swap(pixelColor1, pixelColor2);
    }

    if (point1[2] > point3[2]) {
        std::swap(point1, point3);
        std::swap(pixelColor1, pixelColor3);
    }

    if (point2[2] > point3[2]) {
        std::swap(point2, point3);
        std::swap(pixelColor2, pixelColor3);
    }

    if (point1[2] >= 0) {
        return;
    } else if (point1[2] < 0 && point2[2] >= 0) {
        point2 = point1 + (point2 - point1) * (-point1[2]) / (point2[2] - point1[2]) + EPS;
        point3 = point1 + (point3 - point1) * (-point1[2]) / (point3[2] - point1[2]) + EPS;
    } else if (point2[2] < 0 && point3[2] >= 0) {
        Vector4d point4 = point1 + (point3 - point1) * (-point1[2]) / (point3[2] - point1[2]) + EPS;
        //Uint32 pixelColor4 = screen->mixColors(pixelColor1, pixelColor3, (-point1[2]) / (point3[2] - point1[2]));
        Vector4d point5 = point2 + (point3 - point2) * (-point2[2]) / (point3[2] - point2[2]) + EPS;
        //Uint32 pixelColor5 = screen->mixColors(pixelColor2, pixelColor3, (-point2[2]) / (point3[2] - point2[2]));
        drawTriangle(point1, point2, point4, pixelColor1, pixelColor2, pixelColor3);
        drawTriangle(point2, point4, point5, pixelColor1, pixelColor2, pixelColor3);
        return;
    }

    point1 = camera->fullProject(point1);
    point2 = camera->fullProject(point2);
    point3 = camera->fullProject(point3);

    Vector3d info1 = screen->getPixel(point1);
    Vector3d info2 = screen->getPixel(point2);
    Vector3d info3 = screen->getPixel(point3);
    screen->drawTriangle(info1, info2, info3, pixelColor1, pixelColor2, pixelColor3, intensity);
}

void Renderer::drawEdge(Object &obj, std::pair<int, int> edgeBorders) {
    Vector4d shift = obj.getPosition();
    for (int i = edgeBorders.first + 1; i < edgeBorders.second; ++i) {
        drawTriangle(shift + obj[edgeBorders.first].first, shift + obj[i].first, shift + obj[i + 1].first, obj[edgeBorders.first].second, obj[i].second, obj[i + 1].second);
    }
}

void Renderer::drawObject(Object &obj) {
    int edgesCnt = obj.getCnt();
    for (int i = 0; i < edgesCnt; ++i) {
        drawEdge(obj, obj.getEdge(i));
    }
}

void Renderer::update() {
    screen->userView();
}

void Renderer::addObject(Object object) {
    world->addObject(object);
}

void Renderer::addFile(const std::string& s, Vector4d pos) {
    ObjectParser parser;
    addObject(parser.parse(s, pos));
}

void Renderer::moveFromBasis(double coef, int pos) {
    camera->moveFromBasis(coef, pos);
}

void Renderer::moveCamera(Vector4d move) {
    camera->move(move);
}

void Renderer::rotateCamera(double angle1, double angle2) {
    camera->rotateBasis(angle1 / screen->getWidth() * camera->getXAngle(), angle2 / screen->getHeight() * camera->getXAngle());
}

void Renderer::render() {
    screen->clear();
    int sz = world->getSize();
    for (int i = 0; i < sz; ++i) {
        drawObject(world->getObject(i));
    }
    update();
}
