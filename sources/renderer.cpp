#include "renderer.h"
#include "SDL.h"

#include <iostream>

Renderer::Renderer(Camera camera, int width, int height) : camera(camera), screen(this, width, height) {};

void Renderer::getInfo() {
    camera.printFullBasis();
    camera.printCubeTransform();
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

void Renderer::drawTriangle(Vector4d point1, Vector4d point2, Vector4d point3, Uint32 pixelColor1, Uint32 pixelColor2, Uint32 pixelColor3) {
    Vector3d info1 = screen.getPixel(point1);
    Vector3d info2 = screen.getPixel(point2);
    Vector3d info3 = screen.getPixel(point3);
    screen.drawTriangle(info1, info2, info3, pixelColor1, pixelColor2, pixelColor3);
}

void Renderer::updateCamera() {
    camera.createFullBasis();
    camera.updateCubeTransform(-1, 1, -1, 1);
}

void Renderer::update() {
    screen.update();
}
