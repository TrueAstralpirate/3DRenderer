#include <vector>

#include "world.h"

namespace Project {

World::World() {};

void World::addObject(Object object) {
    objects.push_back(std::move(object));
}

Object& World::getObject(int pos) {
    return objects[pos];
}

int World::getSize() {
    return (int) objects.size();
}

}
