#pragma once

#include "geom.hh"
#include "term.hh"

#include <vector>


// flattened, colour-per-pixel buffer of a frame,
// acting as if we were just a normal display;
// (TODO #finish) use so that we can worry about
// how to map this to canvases (eg. dithering,
// colour-resolution) separately from how to draw
// the scene.
struct Framebuffer {
    Framebuffer (TermInfo const &t);

    void set (int64_t x, int64_t y,
              rgb col = { 1, 1, 1 });
    void set (pixel p,
              rgb col = { 1, 1, 1 })
        { set(p.x(), p.y(), col); }
    void clear ();

    rgb const &at (int64_t x, int64_t y) const;
    rgb &at (int64_t x, int64_t y);

    inline rgb const &at (pixel p) const
        { return at(p.x(), p.y()); }
    inline rgb &at (pixel p)
        { return at(p.x(), p.y()); }

    int64_t w, h;
    rgb bg;
    std::vector<rgb> data;
};
