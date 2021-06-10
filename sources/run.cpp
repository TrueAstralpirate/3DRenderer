#include "math.h"

#include "application.h"

#include <iostream>

int main(int argc, char* argv[]) {
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
        std::cout << "Can't initialize SDL\n";
        exit(1);
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);

    Project::Application app(argv);
    app.run();

    SDL_Quit();
    return 0;
}
