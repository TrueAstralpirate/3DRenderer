#include <vector>

#include "SDL.h"
#include "Dense"

using namespace Eigen;

class Object {
public:

    Object(Vector4d start);

    Vector4d getPosition();

    int getCnt();

    void addPoint(Vector4d point, Uint32 color);
    void addToEdges(int node);

    void addEdge();

    std::pair<Vector4d, Uint32>& operator[](int pos);

    std::pair<int, int> getEdge(int edge);
private:
    Vector4d position;
    std::vector<std::pair<Vector4d, Uint32>> nodes;
    std::vector<int> edges;
    std::vector<int> order;
};