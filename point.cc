#include "point.hh"


Pixel Point::to_pixel () const {
    return { static_cast<int64_t>(x),
             static_cast<int64_t>(y) };
}

void Point::draw (CharGrid &cg) const {
    cg.set(to_pixel());
}
