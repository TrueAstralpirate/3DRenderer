#include <vector>

#include "world.h"

namespace Project {

World::World() {};

void World::addObject(Object object) {
    objects.push_back(std::move(object));
}

Object& World::getObject(int pos) {
    assert(pos >= 0 && "Negative object");
    return objects[pos];
}

int World::getSize() {
    return (int) objects.size();
}

}
