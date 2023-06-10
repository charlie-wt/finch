#pragma once

#include "draw.hh"
#include "triangle.hh"

#include <vector>


struct Mesh {
    template <typename Canvas>
    void draw (Canvas &canvas, Cam const &cam,
               DrawMode mode) const {
        switch (mode) {
        case DrawMode::FILL:
            draw_fill(canvas, cam); break;
        case DrawMode::LINE:
            draw_lines(canvas, cam); break;
        }
    }

    template <typename Canvas>
    void draw_fill (Canvas &canvas,
                    Cam const &cam) const {
        for (auto t : tris) {
            t += origin;
            t.draw_fill(canvas, cam);
        }
    }

    template <typename Canvas>
    void draw_lines (Canvas &canvas,
                     Cam const &cam) const {
        for (auto t : tris) {
            t += origin;
            t.draw_lines(canvas, cam);
        }
    }

    Mesh& rotate (vec3 axis, double degs);

    // move all the points, but not the origin
    Mesh& offset (vec3 change);

    // move the origin, from which points are
    // rotated & drawn
    Mesh& operator+= (vec3 change);
    Mesh& operator-= (vec3 change);

    std::vector<Tri> tris;
    vec3 origin;
};

Mesh offset (Mesh s, vec3 change);

Mesh operator+ (Mesh s, vec3 offset);
Mesh operator- (Mesh s, vec3 offset);
