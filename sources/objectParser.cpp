#include "objectParser.h"

namespace Project {

Object ObjectParser::parse(const std::string& file, Eigen::Vector4d pos) {
    std::fstream parseFile;
    parseFile.open(file, std::ios::in);

    Object res(pos);
    if (!parseFile) {
        std::cout << "No such file\n";
        return res;
    } else {
        std::string s;
        while (!parseFile.eof()) {
            getline(parseFile, s);
            std::stringstream fileStream(s);
            char c;
            fileStream >> c;
            if (c == 'v') {
                int r = 255;
                int g = 255;
                int b = 255;
                Eigen::Vector4d cur;
                fileStream >> cur[0] >> cur[1] >> cur[2] >> r >> g >> b;
                cur[3] = 1;
                res.addPoint(cur, (b << 16) | (g << 8) | r);
            } else if (c == 'f') {
                res.addEdge();
                int x;
                while (fileStream >> x) {
                    res.addToEdges(x - 1);
                }
            }
        }
    }
    parseFile.close();
    return res;
}

}
