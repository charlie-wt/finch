#pragma once

#include "geom.hh"
#include "term.hh"

#include <limits>
#include <vector>


// NOTE: this records z-values from homogeneous
// coord space (defined to be looking into +z), so
// visible values will be in (0,1].
struct DepthBuffer {
    DepthBuffer (pixel const &dims_);
                 // TODO #remove
                 // double near = 0.001,
                 // double far = 1);

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
    // TODO #remove
    // // TODO #cleanup: feel like usually these are
    // // properties of the camera
    // double near, far;
};
