#pragma once

#include "depth.hh"
#include "geom.hh"
#include "term.hh"

#include <string>
#include <vector>


// note, by default (on android) braille is
// broken; characters with only dots on the right
// side display their dots on the left side
// instead.
//
// see https://github.com/google/fonts/issues/3935

struct Block {
    wchar_t to_char () const;

    // essentially a bitfield, top-left to
    // bottom-right, vertical-first.
    uint8_t data;
};

struct BrailleCanvas {
    BrailleCanvas (pixel const &dims_);

    static constexpr vec2 scale { 2, 4 };

    void set (int64_t x, int64_t y,
              bool on = true);
    void set (pixel p,
              bool on = true)
        { set(p.x(), p.y(), on); }
    void draw () const;
    void clear ();

    pixel char_dims, dims;
    std::vector<Block> data;
};
