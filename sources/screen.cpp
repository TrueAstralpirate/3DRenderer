#include <algorithm>
#include <iostream>

#include "screen.h"

#include "SDL.h"

const double EPS = 0.0000000001;

Screen::Screen(Renderer *renderer, int width, int height) : renderer(renderer), zBuffer(width, height) {
    window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    clear();
};

Screen::~Screen() {
    SDL_DestroyWindow(window);
}

void Screen::setPixelColor(int x, int y, Uint32 pixelColor) {
    Uint32 *target_pixel = (Uint32 *) ((Uint8 *) surface->pixels + y * surface->pitch + x * sizeof(*target_pixel));
    *target_pixel = pixelColor;
}

void Screen::setPixelZ(int x, int y, double z) {
    zBuffer(y, x) = z;
}

void Screen::clear() {
    size_t height = zBuffer.getHeight();
    size_t width = zBuffer.getWidth();
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            setPixelColor(x, y, 0);
            setPixelZ(x, y, 20);
        }
    }
}

void Screen::update() {
    SDL_UpdateWindowSurface(window);
}

void Screen::drawPoint(int x, int y, double z, Uint32 pixelColor) {
    if (x < 0 || x >= zBuffer.getWidth() || y < 0 || y >= zBuffer.getHeight()) {
        return;
    }
    if (z > zBuffer(y, x) + EPS) {
        //std::cout << "HERE " << z << ' ' << zBuffer(y, x) << '\n';
        return;
    }
    //std::cout << "DRAWING " << x << ' ' << y << ' ' << z << ' ' << pixelColor << '\n';
    setPixelColor(x, y, pixelColor);
    setPixelZ(x, y, z);
    //update();
}

Vector3d Screen::getPixel(Vector4d point) {
    if (point[3] == 0.0) {
        return Vector3d(-1, -1, 3);
    }
    /*std::cout << "TRANSFORM TO PIXEL\n";
    std::cout << point << "\n";*/
    Vector3d result = point.head(3) / point[3];
    double pixelWidth = 2.0 / (double) zBuffer.getWidth();
    double pixelHeight = 2.0 / (double) zBuffer.getHeight();
    result[0] = floor((result[0] + 1.0) / pixelWidth);
    result[1] = floor((result[1] + 1.0) / pixelHeight);
    //std::cout << result << '\n';
    return result;
}

Uint32 Screen::mixColors(Uint32 color1, Uint32 color2, double t) {
    Uint32 c1 = (double) (color1 & 255) * (1.0 - t) + (double) (color2 & 255) * t;
    Uint32 c2 = (double) ((color1 >> 8) & 255) * (1.0 - t) + (double) ((color2 >> 8) & 255) * t;
    Uint32 c3 = (double) ((color1 >> 16) & 255) * (1.0 - t) + (double) ((color2 >> 16) & 255) * t;
    return (c3 << 16) + (c2 << 8) + c1;
}

void Screen::drawPixelLine(Vector3d point1, Vector3d point2, Uint32 pixelColor1, Uint32 pixelColor2) {
    double width = zBuffer.getWidth();
    double height = zBuffer.getHeight();
    point1[0] = std::max(0.0, std::min(width, point1[0]));
    point1[1] = std::max(0.0, std::min(height, point1[1]));
    point2[0] = std::max(0.0, std::min(width, point2[0]));
    point2[1] = std::max(0.0, std::min(height, point2[1]));
    bool steep = false;
    if (std::abs(point1[0] - point2[0]) < std::abs(point1[1] - point2[1])) {
        std::swap(point1[0], point1[1]);
        std::swap(point2[0], point2[1]);
        steep = true;
    }
    if (point1[0] > point2[0]) {
        std::swap(point1, point2);
        std::swap(pixelColor1, pixelColor2);
    }
    for (int x = point1[0]; x <= point2[0]; ++x) {
        double t = (x - point1[0]) / (double) std::max(1.0, (point2[0] - point1[0]));
        int y = point1[1] * (1.0 - t) + point2[1] * t;
        double z = point1[2] * (1.0 - t) + point2[2] * t;
        Uint32 color = mixColors(pixelColor1, pixelColor2, t);
        //std::cout << "HERE " << x << ' ' << y << ' ' << z << '\n';
        if (steep) {
            drawPoint(y, x, z, color);
        } else {
            drawPoint(x, y, z, color);
        }
    }
    update();
}

void Screen::drawTriangle(Vector3d point1, Vector3d point2, Vector3d point3, Uint32 color1, Uint32 color2, Uint32 color3) {
    //SDL_LockSurface(surface);
    if (point1[1] > point2[1]) {
        std::swap(point1, point2);
        std::swap(color1, color2);
    }
    if (point1[1] > point3[1]) {
        std::swap(point1, point3);
        std::swap(color1, color3);
    }
    if (point2[1] > point3[1]) {
        std::swap(point2, point3);
        std::swap(color2, color3);
    }
    //std::cout << point1 << '\n' << point2 << '\n' << point3 << '\n';
    int triangle_height = point3[1] - point1[1] + 1;
    int lower_height = point2[1] - point1[1] + 1;
    if (lower_height == 0) {
        drawPoint(point1[0], point1[1], point1[2], color1);
    } else {
        int iBorder = std::min((int) zBuffer.getHeight(), (int) point2[1]);
        for (int i = std::max(0, (int) point1[1]); i <= iBorder; ++i) {
            double k3 = (double) (i - point1[1]) / triangle_height;
            double k2 = (double) (i - point1[1]) / lower_height;
            Vector3d A = point1 + k3 * (point3 - point1);
            Vector3d B = point1 + k2 * (point2 - point1);
            Uint32 colorA = mixColors(color1, color3, k3);
            Uint32 colorB = mixColors(color1, color2, k2);
            if (A[0] > B[0]) {
                std::swap(A, B);
                std::swap(colorA, colorB);
            }
            int jBorder = std::min((int) zBuffer.getWidth(), (int) B[0]);
            for (int j = std::max(0, (int) A[0]); j <= jBorder; ++j) {
                double k = (j - A[0]) / std::max(1.0, B[0] - A[0]);
                Uint32 color = mixColors(colorA, colorB, k);
                drawPoint(j, i, A[2] * (1 - k) + B[2] * k, color);
            }
        }
    }
    int upper_height = point3[1] - point2[1] + 1;
    if (upper_height == 0) {
        drawPoint(point3[0], point3[1], point3[2], color3);
    } else {
        int iBorder = std::min((int) zBuffer.getHeight(), (int) point3[1]);
        for (int i = std::max(0, (int) point2[1]); i <= iBorder; ++i) {
            double k3 = (double) (i - point1[1]) / triangle_height;
            double k2 = (double) (i - point2[1]) / upper_height;
            Vector3d A = point1 + k3 * (point3 - point1);
            Vector3d B = point2 + k2 * (point3 - point2);
            Uint32 colorA = mixColors(color1, color3, k3);
            Uint32 colorB = mixColors(color2, color3, k2);
            if (A[0] > B[0]) {
                std::swap(A, B);
                std::swap(colorA, colorB);
            }
            int jBorder = std::min((int) zBuffer.getWidth(), (int) B[0]);
            for (int j = std::max(0, (int) A[0]); j <= jBorder; ++j) {
                double k = (j - A[0]) / std::max(1.0, B[0] - A[0]);
                Uint32 color = mixColors(colorA, colorB, k);
                drawPoint(j, i, A[2] * (1 - k) + B[2] * k, color);
            }
        }
    }
    //SDL_UnlockSurface(surface);
    /*drawPoint(point1[0], point1[1], -2, 255 << 8);
    drawPoint(point2[0], point2[1], -2, 255 << 8);
    drawPoint(point3[0], point3[1], -2, 255 << 8);*/
    //update();
}