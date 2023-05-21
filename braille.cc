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
    /* std::cout << "set " << p << " to " << pr(on) << "\n"; */
    Pixel const cell { p.x / 2, p.y / 4 };
    Pixel const inner { p.x % 2, p.y % 4 };
    /* std::cout << "cell: " << cell << "\n"; */
    /* std::cout << "inner: " << inner << "\n"; */

    uint8_t const bit = inner.x == 0
        ? inner.y
        : inner.y + 4;

    int64_t const block_idx = (cell.y * char_w) + cell.x;

    Block const before = data[block_idx];
    Block const after { static_cast<uint8_t>(before.data | uint8_t {on} << bit) };

    /* std::cout << "block " << block_idx << " set to " << after.data << " / " << binary(after.data) << "\n"; */

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
