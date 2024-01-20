#include "braille.hh"

#include <ncursesw/ncurses.h>

#include <algorithm>
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

BrailleCanvas::BrailleCanvas (pixel const &char_dims_)
    : char_dims(char_dims_)
    , dims(char_dims_ * BrailleCanvas::scale)
    , data(char_dims_.x() * char_dims_.y(), { 0 }) {}

void BrailleCanvas::set (int64_t x, int64_t y,
                         bool on) {
    if (x < 0 || y < 0 ||
        x >= dims.x() || y >= dims.y())
        return;

    pixel const cell { x / 2, y / 4 };
    pixel const inner { x % 2, y % 4 };

    uint8_t const bit_idx = inner.y() + 4 * inner.x();

    int64_t const block_idx =
        (cell.y() * char_dims.x()) + cell.x();

    Block const before = data[block_idx];

    uint8_t const bit = uint8_t {1} << bit_idx;
    uint8_t const after_data = on
        ? before.data | bit
        : before.data & ~bit;
    Block const after { after_data };

    data[block_idx] = after;
}

void BrailleCanvas::draw () const {
    for (size_t i = 0; i < data.size(); i++) {
        size_t const x = i % char_dims.x();
        size_t const y = floor(i / char_dims.x());
        const wchar_t s[2] { data[i].to_char(), 0 };
        mvaddwstr(y, x, s);
    }
    ::refresh();
}

void BrailleCanvas::clear () {
    std::fill(data.begin(), data.end(), Block { 0 });
}
