#pragma once

#include <iostream>


struct Pixel {
    template<typename Canvas>
    void draw (Canvas &canvas) const {
        canvas.set(*this);
    }

    int64_t x, y;
};

std::ostream &operator<< (std::ostream &os, Pixel p);

/* TODO #enhancement: more proper 2d/3d geom lib */
struct Point {
    Pixel to_pixel () const;

    template<typename Canvas>
    void draw (Canvas &canvas) const {
        canvas.set(to_pixel());
    }

    double x, y;
};
