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

uint8_t Block::count () const {
    uint8_t count = 0;
    uint8_t d = data;
    while (d > 0) {
        count += d % 2;
        d >>= 1;
    }
    return count;
}

BrailleCanvas::BrailleCanvas (pixel const &char_dims_)
    : char_dims(char_dims_)
    , dims(char_dims_ * BrailleCanvas::scale)
    , data(char_dims_.x() * char_dims_.y(), { 0 }) {}

bool BrailleCanvas::set (int64_t x, int64_t y,
                         bool on) {
    if (x < 0 || y < 0 ||
        x >= dims.x() || y >= dims.y())
        return false;

    pixel const inner { x % 2, y % 4 };
    uint8_t const bit_idx = inner.y() + 4 * inner.x();
    uint8_t const bit = uint8_t {1} << bit_idx;

    if (on)
        block_at(x, y).data |= bit;
    else
        block_at(x, y).data &= ~bit;
    return true;
}

bool BrailleCanvas::set (pixel p, rgb col) {
    if (p.x() < 0 || p.y() < 0 ||
        p.x() >= dims.x() || p.y() >= dims.y())
        return false;

    pixel const inner { p.x() % static_cast<int64_t>(scale.x()),
                        p.y() % static_cast<int64_t>(scale.y()) };
    uint8_t const bit_idx = inner.y() +
                            scale.y() * inner.x();
    uint8_t const bit = uint8_t {1} << bit_idx;

    auto &block = block_at(p.x(), p.y());
    block.data |= bit;

    auto const new_c = block.count();

    if (block.colour)
        block.colour = ((*block.colour * (new_c - 1) + col) / new_c).cast<uint8_t>();
    else
        block.colour = col;

    return true;
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

Block& BrailleCanvas::block_at (int64_t x, int64_t y) {
    return const_cast<Block&>(
        const_cast<const BrailleCanvas*>(this)->
            block_at(x, y));
}
Block const& BrailleCanvas::block_at (int64_t x,
                                      int64_t y) const {
    pixel const cell { static_cast<int64_t>(x / scale.x()),
                       static_cast<int64_t>(y / scale.y()) };

    int64_t const block_idx =
        cell.y() * char_dims.x() + cell.x();

    return data[block_idx];
}
