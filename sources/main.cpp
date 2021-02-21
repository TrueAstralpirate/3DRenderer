#include "renderer.h"
#include <iostream>

int main() {
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        return 1;
    }
    Vector4d pos(0, 0, 0, 0);
    Vector4d v1(1, 0, 0, 0);
    Vector4d v2(0, 1, 0, 0);
    Camera camera(pos, v1, v2);
    Renderer renderer(camera, 500, 500);
    renderer.updateCamera();
    renderer.getInfo();
    double a, b, c;
    std::cin >> a >> b >> c;
    Vector4d point1(a, b, c, 1);
    std::cin >> a >> b >> c;
    Vector4d point2(a, b, c, 1);
    std::cin >> a >> b >> c;
    Vector4d point3(a, b, c, 1);
    renderer.drawTriangle(renderer.getScreenPoint(point1), renderer.getScreenPoint(point2), renderer.getScreenPoint(point3), 255, 255, 255);
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            /*std::cin >> a >> b >> c;
            Vector4d point(a, b, c, 1);
            Vector4d res = renderer.getScreenPoint(point);
            std::cout << res << '\n';
            renderer.drawPoint(res, 255 << 8);*/
            if (e.type == SDL_QUIT){
                quit = true;
            }
            /*if (e.type == SDL_KEYDOWN){
                quit = true;
            }*/
            /*if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }*/
        }
    }
    SDL_Quit();
    return 0;
}