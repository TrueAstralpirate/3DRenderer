#include "renderer.h"
#include "objectParser.h"
#include <iostream>
#include <random>

const double COEF = 1;

int main() {
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        return 1;
    }
    std::random_device rd;
    std::mt19937 gen(rd());

    SDL_SetRelativeMouseMode(SDL_TRUE);
    Vector4d pos(0, -60, 0, 0);
    Vector4d v1(1, 0, 0, 0);
    Vector4d v2(0, 1, 0, 0);
    Camera camera(pos, v1, v2);
    double angleX = camera.getXAngle();
    double angleY = camera.getYAngle();
    Renderer renderer(camera, 1000, 1000 * angleY / angleX);
    renderer.updateCamera();
    renderer.getInfo();
    /*double a, b, c;
    std::cin >> a >> b >> c;
    Vector4d point1(a, b, c, 1);
    std::cin >> a >> b >> c;
    Vector4d point2(a, b, c, 1);
    std::cin >> a >> b >> c;
    Vector4d point3(a, b, c, 1);
    renderer.drawTriangle(renderer.getScreenPoint(point1), renderer.getScreenPoint(point2), renderer.getScreenPoint(point3), 255, 100, 50);*/

    /*double lower_bound = -1;
    double upper_bound = 1;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;*/

    int upper_int = 256 * 256 * 256;
    std::uniform_int_distribution<> uit(0, upper_int);

    /*Object cur(Vector4d(0, 0, 0, 0));
    cur.addPoint(Vector4d(0, 0, -1.5, 1), 255);
    cur.addPoint(Vector4d(0.5, 0, -1.5, 1), 255);
    cur.addPoint(Vector4d(0, 0.5, -1.5, 1), 255);
    cur.addPoint(Vector4d(0.5, 0.5, -1.5, 1), 255);
    cur.addPoint(Vector4d(0, 0, -2.5, 1), 40000);
    cur.addPoint(Vector4d(0.5, 0, -2.5, 1), 40000);
    cur.addPoint(Vector4d(0, 0.5, -2.5, 1), 40000);
    cur.addPoint(Vector4d(0.5, 0.5, -2.5, 1), 40000);
    for (int i = 0; i < 6; ++i) {
        cur.addEdge(uit(gen));
    }
    cur.addToEdge(0, 0);
    cur.addToEdge(0, 1);
    cur.addToEdge(0, 3);
    cur.addToEdge(0, 2);

    cur.addToEdge(1, 4);
    cur.addToEdge(1, 5);
    cur.addToEdge(1, 7);
    cur.addToEdge(1, 6);

    cur.addToEdge(2, 0);
    cur.addToEdge(2, 1);
    cur.addToEdge(2, 5);
    cur.addToEdge(2, 4);

    cur.addToEdge(3, 2);
    cur.addToEdge(3, 3);
    cur.addToEdge(3, 7);
    cur.addToEdge(3, 6);

    cur.addToEdge(4, 0);
    cur.addToEdge(4, 2);
    cur.addToEdge(4, 6);
    cur.addToEdge(4, 4);

    cur.addToEdge(5, 1);
    cur.addToEdge(5, 3);
    cur.addToEdge(5, 7);
    cur.addToEdge(5, 5);*/

    ObjectParser parser;

    Object cur = parser.parse("diamond.obj");

    renderer.addObject(cur);

    renderer.render();


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
                std::cout << e.motion.xrel << ' ' << e.motion.yrel << '\n';
                double angle1 = e.motion.xrel / 1000.0 * angleX;
                double angle2 = e.motion.yrel / 1000.0 * angleX;
                renderer.rotateCamera(angle1, angle2);
                renderer.updateCamera();
                //renderer.getInfo();
                renderer.render();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
                break;
                /*renderer.moveCamera(MOVEFORWARD);
                renderer.render();*/
                /*double a, b;
                a = unif(re);
                b = unif(re);
                Uint32 c1 = uit(re);
                Vector4d p1(a, b, -1.5, 1);
                a = unif(re);
                b = unif(re);
                Uint32 c2 = uit(re);
                Vector4d p2(a, b, -1.5, 1);
                a = unif(re);
                b = unif(re);
                Uint32 c3 = uit(re);
                Vector4d p3(a, b, -2.5, 1);
                Object newObj;
                newObj.addPoint(p1, c1);
                newObj.addPoint(p2, c2);
                newObj.addPoint(p3, c3);
                newObj.addEdge();
                newObj.addToEdge(0, 0);
                newObj.addToEdge(0, 1);
                newObj.addToEdge(0, 2);
                renderer.addObject(newObj);
                renderer.render();*/
                //renderer.drawTriangle(p1, p2, p3, c1, c2, c3);
                //std::cout << "NEXT\n";
                //quit = true;
            }
        }
    }
    SDL_Quit();
    return 0;
}