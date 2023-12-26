#pragma once

#include "draw.hh"
#include "triangle.hh"

#include <vector>


struct Mesh {
    static Mesh from_verts (
        std::vector<vec3> const &verts);
    static Mesh from_tris (
        std::vector<Tri> const &tris);

    static Mesh box(double width,
                    double height,
                    double depth);
    static Mesh cube(double length);

    template <typename Canvas>
    void draw (Canvas &canvas, Cam const &cam,
               DrawMode mode) const {
        switch (mode) {
        case DrawMode::FILL:
            draw_fill(canvas, cam); break;
        case DrawMode::LINE:
            draw_line(canvas, cam); break;
        }
    }

    template <typename Canvas>
    void draw_fill (Canvas &canvas,
                    Cam const &cam) const {
        for (auto const &idx : idxs) {
            ::draw_fill({ verts[idx[0]] + origin,
                          verts[idx[1]] + origin,
                          verts[idx[2]] + origin },
                        canvas, cam);
        }
    }

    template <typename Canvas>
    void draw_line (Canvas &canvas,
                    Cam const &cam) const {
        for (auto const &idx : idxs) {
            ::draw_line({ verts[idx[0]] + origin,
                          verts[idx[1]] + origin,
                          verts[idx[2]] + origin },
                        canvas, cam);
        }
    }

    Mesh& rotate(vec3 axis, double degs);

    // move all the points, but not the origin
    Mesh& offset(vec3 change);

    // move the origin, from which points are
    // rotated & drawn
    Mesh& operator+=(vec3 change);
    Mesh& operator-=(vec3 change);

    std::vector<vec3> verts;
    std::vector<vec3> norms;
    std::vector<vec3i> idxs;
    vec3 origin;
};

Mesh offset(Mesh s, vec3 change);

Mesh operator+(Mesh s, vec3 offset);
Mesh operator-(Mesh s, vec3 offset);
