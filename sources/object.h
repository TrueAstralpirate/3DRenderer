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
    void addToEdge(int edge, int node);

    void addEdge(void);

    std::vector<std::pair<Vector4d, Uint32>> getEdge(int edge);
private:
    Vector4d position;
    std::vector<std::pair<Vector4d, Uint32>> nodes;
    std::vector<std::vector<int> > edges;
};