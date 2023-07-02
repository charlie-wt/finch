#pragma once

#include "geom.hh"
#include "term.hh"

#include <limits>
#include <vector>


struct DepthBuffer {
    DepthBuffer (TermInfo const &t,
                 double near
                     = -999,
                     /* = std::numeric_limits<double>::max(), */
                 double far
                     = 999);
                     /* = std::numeric_limits<double>::min()); */

    bool set (pixel p, double depth = 0.);
    void clear ();

    double const &at (pixel p) const;
    double &at (pixel p);

    int64_t w, h;
    std::vector<double> data;
    double near, far;
};
