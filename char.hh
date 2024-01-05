#pragma once

#include "depth.hh"
#include "geom.hh"
#include "term.hh"

#include <string>


struct CharCanvas {
    CharCanvas (TermInfo const &t);

    void set (int64_t x, int64_t y, char c = '*');
    void set (int64_t x, int64_t y, double depth = 0);
    void set (pixel p, char c = '*')
        { set(p.x(), p.y(), c); }
    void set (pixel p, double depth = 0)
        { set(p.x(), p.y(), depth); }
    void draw () const;
    void clear ();

    int64_t w, h;
    std::string data;
    /* TODO #remove */
    DepthBuffer depth_buf;
};
