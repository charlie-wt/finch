#include "char.hh"

#include <iostream>


CharCanvas::CharCanvas (TermInfo const &t)
    : w(t.w)
    , h(t.h)
    , data(w * h, ' ') {}

void CharCanvas::set (Pixel p, char c) {
    data[(p.y * w) + p.x] = c;
}

void CharCanvas::draw () const {
    std::cout << "\r" << data << std::flush;
}

void CharCanvas::clear () {
    data = std::string(w * h, ' ');
}
