#include <vector>

#include "objectParser.h"

class World {
public:
    World();

    void addObject(Object object);
    Object& getObject(int pos);

    int getSize();

private:
    std::vector<Object> objects;
};
