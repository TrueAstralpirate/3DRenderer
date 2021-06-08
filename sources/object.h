#include <vector>

#include "SDL.h"
#include "Dense"

namespace Project {

class Object {
public:

    Object(Eigen::Vector4d start);

    Eigen::Vector4d getPosition();

    int getCnt();

    void addPoint(Eigen::Vector4d point, Uint32 color);
    void addToEdges(int node);

    void addEdge();

    std::pair<Eigen::Vector4d, Uint32>& operator[](int pos);

    std::pair<int, int> getEdge(int edge);
private:
    Eigen::Vector4d position;
    std::vector<std::pair<Eigen::Vector4d, Uint32>> nodes;
    std::vector<int> edges;
    std::vector<int> order;
};

}
