#include "renderer.h"
#include "SDL.h"

#include <iostream>

const Vector4d EPS = Vector4d(0, 0, -0.1, 0);

Renderer::Renderer(Camera camera, int width, int height) : camera(camera), screen(this, width, height) {
    update();
};

void Renderer::getInfo() {
    std::cout << "HERE\n";
    camera.printFullBasis();
    //camera.printCubeTransform();
}

Vector4d Renderer::getScreenPoint(Vector4d point) {
    return camera.transformToCube(point);
}

void Renderer::drawPoint(Vector4d point, Uint32 pixelColor) {
    if (point[3] == 0) {
        return;
    }
    Vector3d info = screen.getPixel(point);
    screen.drawPoint(info[0], info[1], info[2], pixelColor);
}

void Renderer::drawLine(Vector4d point1, Vector4d point2, Uint32 pixelColor1, Uint32 pixelColor2) {
    Vector3d info1 = screen.getPixel(point1);
    Vector3d info2 = screen.getPixel(point2);
    screen.drawPixelLine(info1, info2, pixelColor1, pixelColor2);
}

void Renderer::drawTriangle(Vector4d point1, Vector4d point2, Vector4d point3, Uint32 pixelColor1, Uint32 pixelColor2, Uint32 pixelColor3, Uint32 color) {
    /*std::cout << point1[0] << ' ' << point1[1] << ' ' << point1[2] << '\n';
    std::cout << point2[0] << ' ' << point2[1] << ' ' << point2[2] << '\n';
    std::cout << point3[0] << ' ' << point3[1] << ' ' << point3[2] << '\n';*/
    point1 = camera.transformPointToCameraBasis(point1);
    point2 = camera.transformPointToCameraBasis(point2);
    point3 = camera.transformPointToCameraBasis(point3);

    if (point1[2] > point2[2]) {
        std::swap(point1, point2);
    }

    if (point1[2] > point3[2]) {
        std::swap(point1, point3);
    }

    if (point2[2] > point3[2]) {
        std::swap(point2, point3);
    }

    if (point1[2] >= 0) {
        return;
    } else if (point1[2] < 0 && point2[2] >= 0) {
        point2 = point1 + (point2 - point1) * (-point1[2]) / (point2[2] - point1[2]) + EPS;
        point3 = point1 + (point3 - point1) * (-point1[2]) / (point3[2] - point1[2]) + EPS;
    } else if (point2[2] < 0 && point3[2] >= 0) {
        Vector4d point4 = point1 + (point3 - point1) * (-point1[2]) / (point3[2] - point1[2]) + EPS;
        Uint32 pixelColor4 = screen.mixColors(pixelColor1, pixelColor3, (-point1[2]) / (point3[2] - point1[2]));
        Vector4d point5 = point2 + (point3 - point2) * (-point2[2]) / (point3[2] - point2[2]) + EPS;
        Uint32 pixelColor5 = screen.mixColors(pixelColor2, pixelColor3, (-point2[2]) / (point3[2] - point2[2]));
        drawTriangle(point1, point2, point4, pixelColor1, pixelColor2, pixelColor4, color);
        drawTriangle(point2, point4, point5, pixelColor2, pixelColor4, pixelColor5, color);
        return;
    }

    point1 = camera.fullProject(point1);
    point2 = camera.fullProject(point2);
    point3 = camera.fullProject(point3);

    Vector3d info1 = screen.getPixel(point1);
    Vector3d info2 = screen.getPixel(point2);
    Vector3d info3 = screen.getPixel(point3);
    //std::cout << "WTF\n";
    screen.drawTriangle(info1, info2, info3, pixelColor1, pixelColor2, pixelColor3, color);
}

void Renderer::drawEdge(Vector4d shift, std::vector<std::pair<Vector4d, Uint32>> edge, Uint32 color) {
    for (int i = 1; i < (int) edge.size() - 1; ++i) {
        //std::cout << "DRAW TRIANGLE " << i << '\n';
        drawTriangle(shift + edge[0].first, shift + edge[i].first, shift + edge[i + 1].first, edge[0].second, edge[i].second, edge[i + 1].second, color);
    }
}

void Renderer::drawObject(Object obj) {
    int edgesCnt = obj.getCnt();
    Vector4d shift = obj.getPosition();
    for (int i = 0; i < edgesCnt; ++i) {
        //std::cout << "DRAW EDGE " << i << '\n';
        drawEdge(shift, obj.getEdge(i), obj.getCol(i));
    }
}

void Renderer::updateCamera() {
    camera.createFullBasis();
    //camera.updateCubeTransform(-1, 1, -1, 1);
}

void Renderer::update() {
    screen.update();
}

void Renderer::addObject(Object object) {
    world.addObject(object);
}

void Renderer::moveFromBasis(const double coef, const int pos) {
    camera.moveFromBasis(coef, pos);
}

void Renderer::moveCamera(const Vector4d move) {
    camera.move(move);
}

void Renderer::rotateCamera(double angle1, double angle2) {
    camera.rotateBasis(angle1, angle2);
}

void Renderer::render() {
    screen.clear();
    int sz = world.getSize();
    for (int i = 0; i < sz; ++i) {
        //std::cout << "RENDER " << i << '\n';
        drawObject(world.getObject(i));
    }
    update();
}
