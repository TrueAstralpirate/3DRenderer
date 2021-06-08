#include <vector>

#include "object.h"
#include "Dense"

namespace Project {

Object::Object(Eigen::Vector4d start) : position(std::move(start)) {};

Eigen::Vector4d Object::getPosition() {
    return position;
}

int Object::getCnt() {
    return (int) order.size();
}

void Object::addPoint(Eigen::Vector4d point, Uint32 color) {
    nodes.emplace_back(point, color);
}

void Object::addToEdges(int node) {
    edges.push_back(node);
}

void Object::addEdge() {
    order.push_back(edges.size());
}

std::pair<Eigen::Vector4d, Uint32>& Object::operator[](int pos) {
    return nodes[edges[pos]];
}

std::pair<int, int> Object::getEdge(int edge) {
    if (edge + 1 == order.size()) {
        return {order[edge], (int) edges.size() - 1};
    }
    return {order[edge], order[edge + 1] - 1};
}

}
