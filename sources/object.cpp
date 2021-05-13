#include <vector>

#include "object.h"
#include "Dense"

using namespace Eigen;

Object::Object(Vector4d start) : position(start) {};

Vector4d Object::getPosition() {
    return position;
}

int Object::getCnt() {
    return (int) edges.size();
}

void Object::addPoint(Vector4d point, Uint32 color) {
    nodes.push_back({point, color});
}

void Object::addToEdge(int edge, int node) {
    edges[edge].first.push_back(node);
}

void Object::addEdge(const Uint32 color) {
    edges.push_back({std::vector<int>(), color});
}

Uint32 Object::getCol(int edge) {
    return edges[edge].second;
}

std::vector<std::pair<Vector4d, Uint32>> Object::getEdge(int edge) {
    std::vector<std::pair<Vector4d, Uint32>> res;
    for (int it : edges[edge].first) {
        res.push_back(nodes[it]);
    }
    return res;
}