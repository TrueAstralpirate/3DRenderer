#include "userControl.h"

#include <string>
#include <iostream>

UserControl::UserControl(Renderer *renderer) : renderer(renderer) {};

void UserControl::addObject() {
    std::string s;
    double x, y, z;
    std::cin >> s >> x >> y >> z;
    Vector4d pos(x, y, z, 0);
    renderer->addFile("Objects/" + s, pos);
}

void UserControl::run() {
    SDL_Event event;
    while (process(event)) {
        renderer->render();
    }
}

void UserControl::handleKeyLeft() {
    renderer->moveFromBasis(-moveSpeed, 0);
}

void UserControl::handleKeyRight() {
    renderer->moveFromBasis(moveSpeed, 0);
}

void UserControl::handleKeyUp() {
    renderer->moveFromBasis(-moveSpeed, 2);
}

void UserControl::handleKeyDown() {
    renderer->moveFromBasis(moveSpeed, 2);
}

void UserControl::handleKeyPressed(SDL_Keycode key) {
    switch (key) {
        case SDLK_LEFT: handleKeyLeft(); break;
        case SDLK_RIGHT: handleKeyRight(); break;
        case SDLK_UP: handleKeyUp(); break;
        case SDLK_DOWN: handleKeyDown(); break;
        case SDLK_SPACE: addObject();
    }
    renderer->render();
}

void UserControl::handleMouseMotion(double xMove, double yMove) {
    renderer->rotateCamera(xMove, yMove);
    renderer->render();
}

bool UserControl::process(SDL_Event &event) {
    if (!SDL_PollEvent(&event)) {
        return true;
    }
    switch (event.type) {
        case SDL_MOUSEMOTION: handleMouseMotion(event.motion.xrel, event.motion.yrel); break;
        case SDL_KEYDOWN: handleKeyPressed(event.key.keysym.sym); break;
        case SDL_MOUSEBUTTONDOWN: return false;
    }
    return true;
}
