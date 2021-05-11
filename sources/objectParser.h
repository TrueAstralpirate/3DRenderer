#include <string>
#include <fstream>
#include <iostream>

#include "Dense"

class ObjectParser {
public:

    Object parse(std::string file) {
        std::fstream parseFile;
        parseFile.open(file, std::ios::in);

        Object res(Vector4d(0, 0, 0, 0));
        if (!parseFile) {
            std::cout << "No such file\n";
            return res;
        } else {
            std::string s;
            while (1) {
                getline(parseFile, s);
                if (parseFile.eof()) {
                    break;
                }
                std::stringstream ss(s);
                char c;
                ss >> c;
                if (c == 'v') {
                    Vector4d cur;
                    ss >> cur[0] >> cur[1] >> cur[2] >> cur[3];
                    res.addPoint(cur, 255);
                } else if (c == 'f') {
                    res.addEdge();
                    int last = res.getCnt() - 1;
                    int x;
                    while (ss >> x) {
                        res.addToEdge(last, x - 1);
                    }
                }
            }
        }
        return res;
    }
};