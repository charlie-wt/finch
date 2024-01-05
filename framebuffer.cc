#include "framebuffer.hh"

#include <algorithm>


Framebuffer::Framebuffer (TermInfo const &t)
    : w(t.w)
    , h(t.h)
    , bg ({0, 0, 0})
    , data(w * h, bg) {}

void Framebuffer::set (int64_t x, int64_t y,
                       rgb col) {
    if (x < 0 || y < 0 ||
        x >= w || y >= h)
        return;

    at(x, y) = col;
}

void Framebuffer::clear () {
    std::fill(data.begin(), data.end(), bg);
}

rgb &Framebuffer::at (int64_t x, int64_t y) {
    return data[y * w + x];
}

rgb const &Framebuffer::at (int64_t x,
                            int64_t y) const {
    return data[y * w + x];
}
