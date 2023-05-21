#pragma once

#include "braille.hh"
#include "grid.hh"
#include "point.hh"


struct Line {
    Point start, end;
    void draw (BrailleGrid &cg) const;
};
