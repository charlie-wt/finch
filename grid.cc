#include "grid.hh"

#include <iostream>


void Pixel::draw (CharGrid &cg) const {
    cg.set(*this);
}

CharGrid::CharGrid (TermInfo const &t)
    : w(t.w)
    , h(t.h)
    , data(w * h, ' ') {}

void CharGrid::set (Pixel p, char c) {
    data[(p.y * w) + p.x] = c;
}

void CharGrid::draw () const {
    std::cout << "\r" << data << std::flush;
}

void CharGrid::clear () {
    data = std::string(w * h, ' ');
}
