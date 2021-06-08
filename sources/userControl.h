#include "SDL.h"
#include "Dense"

#include "time.h"

#include "renderer.h"

namespace Project {

class UserControl {
public:
    UserControl();

    UserControl(Renderer *renderer);

    void run();

    void addObject();

    void handleKeyLeft();

    void handleKeyRight();

    void handleKeyUp();

    void handleKeyDown();

    void handleKeyPressed(SDL_Keycode k);

    void handleMouseMotion(double xMove, double yMove);

    bool process(SDL_Event &event);

private:
    static constexpr double moveSpeed = 1.0;

    Renderer *renderer;
};

}
