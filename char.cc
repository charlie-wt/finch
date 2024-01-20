#include "char.hh"

#include <iostream>


CharCanvas::CharCanvas (pixel const &dims_)
    : dims(dims_)
    , data(dims.x() * dims.y(), ' ') {}

void CharCanvas::set (int64_t x, int64_t y,
                      char c) {
    if (x < 0 || y < 0 ||
        x >= dims.x() || y >= dims.y())
        return;
    data[(y * dims.x()) + x] = c;
}

/* void CharCanvas::set (int64_t x, int64_t y, */
/*                       double depth) { */
/*     if (!depth_buf.set(x, y, depth)) */
/*         return; */

/*     int const zi = depth; */
/*     char const ch = zi < 0 */
/*         ? (depth == depth_buf.far ? '.' : 'v') */
/*         : zi > 9 */
/*             ? '^' */
/*             : std::to_string(zi)[0]; */

/*     set(x, y, ch); */
/* } */

void CharCanvas::draw () const {
    std::cout << "\r" << data << std::flush;
}

void CharCanvas::clear () {
    data = std::string(dims.x() * dims.y(), ' ');
}
