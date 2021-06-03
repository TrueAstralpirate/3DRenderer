#include <vector>

template <typename T>
class Buffer {
public:
    Buffer(size_t width, size_t height) : width(width), height(height) {
        data = std::vector<T>(width * height);
    };

    T* getBuffer() {
        return data;
    }

    size_t getWidth() const {
        return width;
    }

    size_t getHeight() const {
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
