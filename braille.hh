#pragma once

#include "grid.hh"
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

struct BrailleGrid {
    BrailleGrid (TermInfo const &t);

    void set (Pixel p, bool on = true);
    void draw () const;
    void clear ();

    int64_t char_w, char_h;
    int64_t w, h; // in braille dots
    std::vector<Block> data;
};
