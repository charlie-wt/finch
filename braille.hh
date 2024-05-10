#pragma once

#include "colour.hh"
#include "depth.hh"
#include "geom.hh"
#include "term.hh"

#include <optional>
#include <string>
#include <vector>


// note, by default (on android) braille is
// broken; characters with only dots on the right
// side display their dots on the left side
// instead.
//
// see https://github.com/google/fonts/issues/3935

struct Block {
    Block(uint8_t data_,
          std::optional<rgb> colour_ = std::nullopt)
        : data(data_)
        , colour(colour_) {}

    wchar_t to_char () const;

    // number of set pixels in the block
    uint8_t count () const;

    // essentially a bitfield, top-left to
    // bottom-right, vertical-first.
    uint8_t data;
    std::optional<rgb> colour;
};

struct BrailleCanvas {
    BrailleCanvas (pixel const &dims_);

    static constexpr vec2 scale { 2, 4 };

    bool set (int64_t x, int64_t y,
              bool on = true);
    bool set (pixel p,
              bool on = true)
        { return set(p.x(), p.y(), on); }
    bool set (pixel p, rgb col);
    bool unset_col  (pixel p);
    void draw () const;
    void clear ();

    pixel char_dims, dims;
    std::vector<Block> data;

private:
    Block& block_at (int64_t x, int64_t y);
    Block const& block_at (int64_t x, int64_t y) const;
};
