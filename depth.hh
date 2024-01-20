#pragma once

#include "geom.hh"
#include "term.hh"

#include <limits>
#include <vector>


struct DepthBuffer {
    DepthBuffer (pixel const &dims_,
                 double near
                     = -999,
                     /* = std::numeric_limits<double>::max(), */
                 double far
                     = 999);
                     /* = std::numeric_limits<double>::min()); */

    bool set (int64_t x, int64_t y,
              double depth = 0.);
    bool set (pixel p, double depth = 0.)
        { return set(p.x(), p.y(), depth); }
    void clear ();

    double const &at (int64_t x, int64_t y) const;
    double &at (int64_t x, int64_t y);

    double const &at (pixel p) const
        { return at(p.x(), p.y()); }
    double &at (pixel p)
        { return at(p.x(), p.y()); }

    pixel dims;
    std::vector<double> data;
    double near, far;
};
