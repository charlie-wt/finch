#pragma once

#include "line.hh"

#include <vector>


struct Shape3d {
    template<typename Canvas>
    void draw (Canvas &canvas) const {
        for (auto const &l : lines)
            l.draw(canvas);
    }

    void rotate (vec3 axis, double degs);

    /* TODO #temp */
    void rot_x (double degs);
    void rot_y (double degs);
    void rot_z (double degs);
    template <typename Canvas>
    Shape3d project (Canvas canvas,
                     double fov, double viewer_dist) {
        std::vector<Line3d> ls = lines;
        ls.reserve(lines.size());
        for (auto &l : ls) {
            l.start = projected(
                l.start, canvas, fov, viewer_dist);
            l.end = projected(
                l.end, canvas, fov, viewer_dist);
        }
        return { ls, origin };
    }

    Shape3d& operator+= (vec3 offset);
    Shape3d& operator-= (vec3 offset);

    std::vector<Line3d> lines;
    vec3 origin;
};

Shape3d operator+ (Shape3d s, vec3 offset);
Shape3d operator- (Shape3d s, vec3 offset);

Shape3d rect (double width, double height);
Shape3d square (double length);
Shape3d box (double width,
             double height,
             double depth);
Shape3d cube (double length);
