#include "depth.hh"

/* TODO #remove */
/* #include <ncursesw/ncurses.h> */

#include <algorithm>


DepthBuffer::DepthBuffer (TermInfo const &t,
                          double near, double far)
    : w(t.w)
    , h(t.h)
    , data(w * h, far)
    , near(near)
    , far(far) { }

bool DepthBuffer::set (int64_t x, int64_t y,
                       double depth) {
    if (x < 0 || y < 0 ||
        x >= w || y >= h)
        return false;

    if (at(x, y) < depth ||
        depth < near ||
        depth > far)
        return false;

    at(x, y) = depth;
    return true;
}

void DepthBuffer::clear () {
    std::fill(data.begin(), data.end(), far);
}

double &DepthBuffer::at (int64_t x, int64_t y) {
    return data[y * w + x];
}

double const &DepthBuffer::at (int64_t x,
                               int64_t y) const {
    return data[y * w + x];
}
