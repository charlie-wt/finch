#include "point.hh"


std::ostream &operator<< (std::ostream &os, Pixel p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

Pixel Point::to_pixel () const {
    return { static_cast<int64_t>(x),
             static_cast<int64_t>(y) };
}
