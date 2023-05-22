#pragma once

#include "point.hh"
#include "term.hh"
#include <string>


std::ostream &operator<< (std::ostream &os, Pixel p);

/* TODO #enhancement: z-buffers, etc. */
struct CharCanvas {
    CharCanvas (TermInfo const &t);

    void set (Pixel p, char c = '*');
    void draw () const;
    void clear ();

    int64_t w, h;
    std::string data;
};
