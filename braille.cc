#include "braille.hh"


wchar_t Block::to_char () const {
    // unfortunately, the braille dots don't tick
    // up in a completely straightforward way; the
    // pattern is:
    // 1 4
    // 2 5
    // 3 6
    // 7 8
    // ...so we need to do some bit-twiddling.
    uint8_t res = data;
    res &= 0b10000111;
    res |= (data & 0b01110000) >> 1;
    res |= (data & 0b00001000) << 3;

    return static_cast<wchar_t>(10240 + res);
}

BrailleGrid::BrailleGrid (TermInfo const &t)
    : w(t.w)
    , h(t.h)
    , data(w * h, ' ') {}
