#include "userControl.h"

namespace Project {

class Application {
public:
    Application(char* argv[]);

    void run();

private:
    Camera camera;
    Screen screen;
    World world;
    Renderer renderer;
    UserControl control;
};

}
