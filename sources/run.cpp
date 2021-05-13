#include "renderer.h"
#include "objectParser.h"
#include <iostream>
#include <random>

const double COEF = 5;

int main(int argc, char* argv[]) {
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        return 1;
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    std::random_device rd;
    std::mt19937 gen(rd());

    double x = strtod(argv[1], NULL);
    double y = strtod(argv[2], NULL);
    double z = strtod(argv[3], NULL);
    int width = strtoll(argv[4], NULL, 10);
    int height = strtoll(argv[5], NULL, 10);

    Vector4d pos(x, y, z, 0);
    Vector4d v1(1, 0, 0, 0);
    Vector4d v2(0, 1, 0, 0);
    Camera camera(pos, v1, v2);
    double angleX = camera.getXAngle();
    double angleY = camera.getYAngle();
    Renderer renderer(camera, width, height);
    renderer.updateCamera();

    int upper_int = 256 * 256 * 256;
    std::uniform_int_distribution<> uit(0, upper_int);

    ObjectParser parser;

    Object cur = parser.parse("diamond.obj");

    renderer.addObject(cur);

    renderer.render();

    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT: renderer.moveFromBasis(-COEF, 0); break;
                    case SDLK_RIGHT: renderer.moveFromBasis(COEF, 0); break;
                    case SDLK_UP: renderer.moveFromBasis(-COEF, 2); break;
                    case SDLK_DOWN: renderer.moveFromBasis(COEF, 2); break;
                }
                renderer.render();
            }
            if (e.type == SDL_MOUSEMOTION) {
                double angle1 = e.motion.xrel / 1000.0 * angleX;
                double angle2 = e.motion.yrel / 1000.0 * angleX;
                renderer.rotateCamera(angle1, angle2);
                renderer.updateCamera();
                renderer.render();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
                break;
            }
        }
    }
    SDL_Quit();
    return 0;
}