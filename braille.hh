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
    BrailleCanvas (TermInfo const &t);

    void set (int64_t x, int64_t y,
              double depth = 0,
              bool on = true);
    void set (pixel p,
              double depth = 0,
              bool on = true)
        { set(p.x(), p.y(), depth, on); }
    void draw () const;
    void clear ();

    int64_t char_w, char_h;
    int64_t w, h; // in braille dots
    std::vector<Block> data;
    /* TODO #remove */
    DepthBuffer depth_buf;
};
