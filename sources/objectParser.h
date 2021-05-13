#include <string>
#include <fstream>
#include <iostream>
#include <random>

#include "Dense"

class ObjectParser {
public:

    Object parse(std::string file) {
        std::random_device rd;
        std::mt19937 gen(rd());

        int upper_int = 256 * 256 * 256;
        std::uniform_int_distribution<> uit(0, upper_int);

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
                    cur[3] = 1;
                    res.addPoint(cur, uit(gen));
                } else if (c == 'f') {
                    res.addEdge(uit(gen));
                    int last = res.getCnt() - 1;
                    int x;
                    while (ss >> x) {
                        res.addToEdge(last, x - 1);
                        //std::cout << last << ' ' << x - 1 << '\n';
                    }
                }
            }
        }
        return res;
    }
};