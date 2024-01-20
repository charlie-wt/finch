#pragma once

#include "depth.hh"
#include "geom.hh"
#include "term.hh"

#include <string>


struct CharCanvas {
    CharCanvas (pixel const &dims_);

    static constexpr vec2 scale { 1, 1 };

    void set (int64_t x, int64_t y, char c = '*');
    void set (pixel p, char c = '*')
        { set(p.x(), p.y(), c); }
    void draw () const;
    void clear ();

    pixel dims;
    std::string data;
};
