#pragma once

#include "grid.hh"


/* TODO #enhancement: more proper 2d/3d geom lib */
struct Point {
    Pixel to_pixel () const;
    void draw (CharGrid &cg) const;

    double x, y;
};
