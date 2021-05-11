#include <vector>

#include "world.h"

void World::addObject(Object object) {
    objects.push_back(object);
}

Object World::getObject(int pos) {
    return objects[pos];
}

int World::getSize() {
    return (int) objects.size();
}