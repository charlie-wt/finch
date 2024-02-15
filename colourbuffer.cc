#include "colourbuffer.hh"

#include <algorithm>


ColourBuffer::ColourBuffer (pixel const &canvas_dims)
    : dims(canvas_dims)
    , bg ({0, 0, 0})
    , data(dims.x() * dims.y(), bg) {}

void ColourBuffer::set (int64_t x, int64_t y,
                        rgb col) {
    if (x < 0 || y < 0 ||
        x >= dims.x() || y >= dims.y())
        return;

    at(x, y) = col;
}

void ColourBuffer::clear () {
    std::fill(data.begin(), data.end(), bg);
}

rgb &ColourBuffer::at (int64_t x, int64_t y) {
    return data[y * dims.x() + x];
}

rgb const &ColourBuffer::at (int64_t x,
                             int64_t y) const {
    return data[y * dims.x() + x];
}
