#pragma once

#include "depth.hh"
#include "geom.hh"
#include "term.hh"

#include <string>


struct CharCanvas {
    CharCanvas (TermInfo const &t);

    void set (pixel p, char c = '*');
    void set (pixel p, double depth);
    void draw () const;
    void clear ();

    int64_t w, h;
    std::string data;
    DepthBuffer db;
};
