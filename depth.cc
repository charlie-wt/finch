#include "depth.hh"

#include <ncursesw/ncurses.h>

#include <algorithm>


DepthBuffer::DepthBuffer (TermInfo const &t,
                          double near, double far)
    : w(t.w)
    , h(t.h)
    , data(w * h, far)
    , near(near)
    , far(far) { }

bool DepthBuffer::set (pixel p, double depth) {
    if (at(p) < depth ||
        depth < near ||
        depth > far)
        return false;

    at(p) = depth;
    return true;
}

void DepthBuffer::clear () {
    std::fill(data.begin(), data.end(), far);
}

double &DepthBuffer::at (pixel p) {
    return data[p.y() * w + p.x()];
}

double const &DepthBuffer::at (pixel p) const {
    return data[p.y() * w + p.x()];
}
