#include <vector>

#include "object.h"

class World {
public:
    void addObject(Object object);
    Object getObject(int pos);

    int getSize();

private:
    std::vector<Object> objects;
};