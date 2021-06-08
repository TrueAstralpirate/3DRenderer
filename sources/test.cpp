#include "SDL.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
const int ITER1 = 50;
const int ITER2 = 50;

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 *target_pixel = (Uint32 *)((Uint8 *) surface->pixels + y * surface->pitch + x * sizeof(*target_pixel));
    *target_pixel = pixel;
}

int main (int argc, char ** args) {
    
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        return 1;
    }

    SDL_Surface* screen_surface = NULL;

    SDL_Window* window = NULL;

    window = SDL_CreateWindow("Hello, SDL 2!",SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
    SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        return 1;
    }

    screen_surface = SDL_GetWindowSurface(window);

    SDL_FillRect(screen_surface, NULL, SDL_MapRGB( screen_surface->format, 0, 0, 0));

    SDL_UpdateWindowSurface(window);

    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int a = rand() % 255;
                int b = rand() % 255;
                int c = rand() % 255;
                //int a = 0;
                //int b = 0;
                //int c = 0;

                //SDL_FillRect(screen_surface, NULL, SDL_MapRGB( screen_surface->format, a, b, c));
                Uint32 pixel = (a << 16) + (b << 8) + (c << 0);
                for (int i = 0; i < ITER1; ++i) {
                    for (int j = 0; j < ITER2; ++j) {
                        int x, y;
                        x = rand() % SCREEN_WIDTH;
                        y = rand() % SCREEN_HEIGHT;
                        set_pixel(screen_surface, i, j, pixel);
                    }
                }
                SDL_UpdateWindowSurface(window);
            }
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            /*if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }*/
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
};
