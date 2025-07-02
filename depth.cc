#include "depth.hh"

#include <algorithm>


DepthBuffer::DepthBuffer (pixel const &canvas_dims)
                          // TODO #remove
                          // double near, double far)
    : dims(canvas_dims)
    , data(dims.x() * dims.y(), 1.) { }
    // TODO #remove
    // , near(near)
    // , far(far) { }

bool DepthBuffer::set (int64_t x, int64_t y,
                       double depth) {
    if (x < 0 || y < 0 ||
        x >= dims.x() || y >= dims.y())
        return false;

    if (at(x, y) < depth ||
        depth < 0 || depth > 1.)
        return false;

    at(x, y) = depth;
    return true;
}

void DepthBuffer::clear () {
    std::fill(data.begin(), data.end(), 1.);
}

double &DepthBuffer::at (int64_t x, int64_t y) {
    return data[y * dims.x() + x];
}

double const &DepthBuffer::at (int64_t x,
                               int64_t y) const {
    return data[y * dims.x() + x];
}
