#pragma once

#include "grid.hh"
#include "point.hh"


struct Line {
    Point start, end;
    void draw (CharGrid &cg) const;
};
