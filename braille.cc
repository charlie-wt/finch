#include "braille.hh"
/* #include "util.hh" */

#include <iostream>


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
    : char_w(t.w)
    , char_h(t.h)
    , w(char_w * 2)
    , h(char_h * 4)
    , data(char_w * char_h, { 0 }) {
}

void BrailleGrid::set (Pixel p, bool on) {
    Pixel const cell { p.x / 2, p.y / 4 };
    Pixel const inner { p.x % 2, p.y % 4 };

    uint8_t const bit_idx = inner.y + 4 * inner.x;

    int64_t const block_idx = (cell.y * char_w) + cell.x;

    Block const before = data[block_idx];


    uint8_t const bit = uint8_t {1} << bit_idx;
    uint8_t const after_data = on
        ? before.data | bit
        : before.data & ~bit;
    Block const after { after_data };

    data[block_idx] = after;
}

void BrailleGrid::draw () const {
    std::wstring str(data.size(), L'\u2800');

    for (int i = 0; i < data.size(); i++) {
        str[i] = data[i].to_char();
    }

    std::wcout << "\r" << str << std::flush;
}

void BrailleGrid::clear () {
    data = std::vector<Block>(char_w * char_h, { 0 });
}
