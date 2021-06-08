#include <string>
#include <fstream>
#include <iostream>
#include <random>

#include "Dense"

#include "object.h"

namespace Project {

class ObjectParser {
public:
    Object parse(const std::string& file, Eigen::Vector4d pos);
};

}
