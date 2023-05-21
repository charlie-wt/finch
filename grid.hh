#pragma once

#include "term.hh"
#include <string>


struct CharGrid;

struct Pixel {
    void draw (CharGrid &cg) const;

    int64_t x, y;
};

std::ostream &operator<< (std::ostream &os, Pixel p);

/* TODO #enhancement: z-buffers, etc. */
struct CharGrid {
    CharGrid (TermInfo const &t);

    void set (Pixel p, char c = '*');
    void draw () const;
    void clear ();

    int64_t w, h;
    std::string data;
};
