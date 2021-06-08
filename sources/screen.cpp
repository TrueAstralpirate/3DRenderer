#include <algorithm>
#include <memory>
#include <iostream>

#include "screen.h"

#include "SDL.h"

namespace Project {

Screen::Screen(int width, int height) 
    : width(width), height(height), zBuffer(width, height), cnt(width, height), run(0) {
    assert(width >= 0 && "Negative width");
    assert(height >= 0 && "Negative height");
    window.reset(SDL_CreateWindow("Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN), SDL_DestroyWindow);
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            cnt(y, x) = 0;
        }
    }
    clear();
};

void Screen::setPixelColor(int x, int y, Uint32 pixelColor) {
    Uint32* pixels = (Uint32*) SDL_GetWindowSurface(window.get())->pixels;
    pixels[y * width + x] = pixelColor;
}

void Screen::setPixelZ(int x, int y, double z) {
    zBuffer(y, x) = z;
}

void Screen::clear() {
    SDL_memset(SDL_GetWindowSurface(window.get())->pixels, 0, SDL_GetWindowSurface(window.get())->h * SDL_GetWindowSurface(window.get())->pitch);
    ++run;
}

void Screen::userView() {
    SDL_UpdateWindowSurface(window.get());
}

int Screen::getWidth() {
    return width;
}

int Screen::getHeight() {
    return height;
}

void Screen::drawPoint(const int& x, const int& y, const double& z, const Uint32& pixelColor) {
    cnt(y, x) = run;
    setPixelColor(x, y, pixelColor);
    setPixelZ(x, y, z);
}

Eigen::Vector3d Screen::getPixel(Eigen::Vector4d point) {
    if (point[3] == 0.0) {
        return Eigen::Vector3d(-1, -1, 3);
    }
    Eigen::Vector3d result = point.head(3) / point[3];
    double pixelWidth = 2.0 / zBuffer.getWidth();
    double pixelHeight = 2.0 / zBuffer.getHeight();
    result[0] = floor((result[0] + 1.0) / pixelWidth);
    result[1] = floor((result[1] + 1.0) / pixelHeight);
    return result;
}

Uint32 Screen::intenseColor(Uint32 color, double intensity) {
    Uint32 c1 = (double) (color & 255) * intensity;
    Uint32 c2 = (double) ((color >> 8) & 255) * intensity;
    Uint32 c3 = (double) ((color >> 16) & 255) * intensity;
    return (c3 << 16) | (c2 << 8) | c1;
}


Uint32 Screen::mixColors(Uint32& color1, Uint32& color2, Uint32& color3, const double& a, const double& b, double& c) {
    Uint32 c1 = (double) (color1 & 255) * a + (double) (color2 & 255) * b + (double) (color3 & 255) * c;
    Uint32 c2 = (double) ((color1 >> 8) & 255) * a + (double) ((color2 >> 8) & 255) * b + (double) ((color3 >> 8) & 255) * c;
    Uint32 c3 = (double) ((color1 >> 16) & 255) * a + (double) ((color2 >> 16) & 255) * b + (double) ((color3 >> 16) & 255) * c;
    return (c3 << 16) | (c2 << 8) | c1;
}

double Screen::area(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2) {
    return v1[0] * v2[1] - v2[0] * v1[1];
}

void Screen::drawPixelLine(Eigen::Vector3d point1, Eigen::Vector3d point2, Uint32 pixelColor1, Uint32 pixelColor2) {
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
        Uint32 color = (1 << 24) - 1;
        if (steep) {
            drawPoint(y, x, z, color);
        } else {
            drawPoint(x, y, z, color);
        }
    }
    userView();
}

void Screen::drawHorizontalLine(Eigen::Vector3d& point1, Eigen::Vector3d& point2, Eigen::Vector3d& point3, Uint32& color1, Uint32& color2, Uint32& color3, double& triangle_area, int left, int right, int y) {
    Eigen::Vector2d p(left, y);
    Eigen::Vector2d p1 = p - point1.head(2);
    Eigen::Vector2d p2 = p - point2.head(2);
    Eigen::Vector2d p3 = p - point3.head(2);
    double a = area(p2, p3) * triangle_area;
    double b = area(p1, p3) * triangle_area;
    double c = 1.0 - std::abs(a) - std::abs(b);
    double da = (p3[1] - p2[1]) * triangle_area;
    double db = (p3[1] - p1[1]) * triangle_area;
    Uint32* pixels = (Uint32*) SDL_GetWindowSurface(window.get())->pixels + y * width + left;
    for (int j = left; j <= right; ++j) {
        c = 1.0 - std::abs(a) - std::abs(b);
        double z = std::abs(a) * point1[2] + std::abs(b) * point2[2] + c * point3[2];
        if (cnt(y, j) == run && z > zBuffer(y, j) + EPS) {
            a += da;
            b += db;
            pixels++;
            continue;
        }
        *pixels = mixColors(color1, color2, color3, std::abs(a), std::abs(b), c);
        cnt(y, j) = run;
        zBuffer(y, j) = z;
        a += da;
        b += db;
        pixels++;
    }
}

void Screen::drawTriangle(Eigen::Vector3d& point1, Eigen::Vector3d& point2, Eigen::Vector3d& point3, Uint32& color1, Uint32& color2, Uint32& color3, double intensity) {
    color1 = intenseColor(color1, intensity);
    color2 = intenseColor(color2, intensity);
    color3 = intenseColor(color3, intensity);

    sortPoints(point1, point2, point3, color1, color2, color3);

    double denom = area(point2.head(2) - point1.head(2), point3.head(2) - point1.head(2));
    if (denom == 0) {
        return;
    }
    denom = 1.0 / denom;

    int triangle_height = point3[1] - point1[1] + 1;
    int lower_height = point2[1] - point1[1] + 1;
    if (lower_height == 0) {
        drawPoint(point1[0], point1[1], point1[2], color1);
    } else {
        int iBorder = std::min((int) zBuffer.getHeight() - 1, (int) point2[1]);
        for (int i = std::max(0, (int) point1[1]); i <= iBorder; ++i) {
            double k3 = (double) (i - point1[1]) / triangle_height;
            double k2 = (double) (i - point1[1]) / lower_height;
            Eigen::Vector3d A = point1 + k3 * (point3 - point1);
            Eigen::Vector3d B = point1 + k2 * (point2 - point1);
            if (A[0] > B[0]) {
                std::swap(A, B);
            }
            drawHorizontalLine(point1, point2, point3, color1, color2, color3, denom, std::max(0, (int) A[0]), std::min((int) zBuffer.getWidth() - 1, (int) B[0]), i);
        }
    }
    int upper_height = point3[1] - point2[1] + 1;
    if (upper_height == 0) {
        drawPoint(point3[0], point3[1], point3[2], color3);
    } else {
        int iBorder = std::min((int) zBuffer.getHeight() - 1, (int) point3[1]);
        for (int i = std::max(0, (int) point2[1]); i <= iBorder; ++i) {
            double k3 = (double) (i - point1[1]) / triangle_height;
            double k2 = (double) (i - point2[1]) / upper_height;
            Eigen::Vector3d A = point1 + k3 * (point3 - point1);
            Eigen::Vector3d B = point2 + k2 * (point3 - point2);
            if (A[0] > B[0]) {
                std::swap(A, B);
            }
            drawHorizontalLine(point1, point2, point3, color1, color2, color3, denom, std::max(0, (int) A[0]), std::min((int) zBuffer.getWidth() - 1, (int) B[0]), i);
        }
    }
}

void Screen::sortPoints(Eigen::Vector3d& point1, Eigen::Vector3d& point2, Eigen::Vector3d& point3, Uint32& color1, Uint32& color2, Uint32& color3) {
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
}

}
