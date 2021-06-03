#include <string>
#include <fstream>
#include <iostream>
#include <random>

#include "Dense"

#include "object.h"

class ObjectParser {
public:

    Object parse(const std::string& file, Vector4d pos) {
        std::fstream parseFile;
        parseFile.open(file, std::ios::in);

        Object res(pos);
        if (!parseFile) {
            std::cout << "No such file\n";
            return res;
        } else {
            int cnt = 0;
            std::string s;
            while (1) {
                getline(parseFile, s);
                if (parseFile.eof()) {
                    break;
                }
                std::stringstream fileStream(s);
                char c;
                fileStream >> c;
                if (c == 'v') {
                    cnt = (cnt + 1) % 3;
                    Vector4d cur;
                    fileStream >> cur[0] >> cur[1] >> cur[2] >> cur[3];
                    cur[3] = 1;
                    if (cnt % 3 == 0) {
                        res.addPoint(cur, (1 << 8) - 1);
                    } else if (cnt % 3 == 1) {
                        res.addPoint(cur, (1 << 16) - (1 << 8));
                    } else {
                        res.addPoint(cur, (1 << 24) - (1 << 16));
                    }
                    //res.addPoint(cur, (1 << 24) - 1);
                } else if (c == 'f') {
                    res.addEdge();
                    int x;
                    while (fileStream >> x) {
                        res.addToEdges(x - 1);
                    }
                }
            }
        }
        return res;
    }
};
