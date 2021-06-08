#include <iostream>

#include "application.h"

namespace Project {

Application::Application(char* argv[]) : camera(Eigen::Vector4d(strtod(argv[1], NULL), strtod(argv[2], NULL), strtod(argv[3], NULL), 0),
                                                strtod(argv[4], NULL), strtod(argv[5], NULL), strtod(argv[6], NULL) ),
                                         screen(strtoll(argv[7], NULL, 10), strtoll(argv[8], NULL, 10)),
                                         world(),
                                         renderer(&camera, &screen, &world),
                                         control(&renderer) {
}

void Application::run() {
    renderer.render();
    control.run();
}

}
