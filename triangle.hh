#pragma once

#include "geom.hh"


struct Tri {
    template <typename Canvas>
    void draw_fill (Canvas &canvas,
                    Cam const &cam) const {
        (void)canvas; (void)cam;
    }

    template <typename Canvas>
    void draw_lines (Canvas &canvas,
                     Cam const &cam) const {
        Line3d { verts[0], verts[1] }.draw(canvas, cam);
        Line3d { verts[1], verts[2] }.draw(canvas, cam);
        Line3d { verts[2], verts[0] }.draw(canvas, cam);
    }

    std::array<vec3, 3> verts;
};
