#include <vector>

namespace Project {

template <typename T>
class Buffer {
public:
    Buffer(int width, int height) : width(width), height(height) {
        assert(width >= 0 && "Negative width");
        assert(height >= 0 && "Negative height");
        data = std::vector<T>(width * height);
    };

    T* getBuffer() {
        return data;
    }

    size_t getWidth() {
        return width;
    }

    size_t getHeight() {
        return height;
    }

    T& operator() (size_t row, size_t column) {
        return data[row * width + column];
    }

    const T& operator() (size_t row, size_t column) const {
        return data[row * width + column];
    }
    
private:
    size_t width;
    size_t height;

    std::vector<T> data;
};

}
