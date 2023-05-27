#pragma once

#include "line.hh"

#include <vector>


struct Shape3d {
    template<typename Canvas>
    void draw (Canvas &canvas,
               double fov,
               double viewer_dist) const {
        for (auto const &l : lines)
            l.draw(canvas, fov, viewer_dist);
    }

    void rotate (vec3 axis, double degs);

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
