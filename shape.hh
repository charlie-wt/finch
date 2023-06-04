#pragma once

#include "line.hh"

#include <vector>


struct Shape3d {
    template<typename Canvas>
    void draw (Canvas &canvas,
               double fov,
               double viewer_dist) const {
        for (auto l : lines) {
            l += origin;
            l.draw(canvas, fov, viewer_dist);
        }
    }

    Shape3d& rotate (vec3 axis, double degs);

    // move all the points, but not the origin
    Shape3d& offset (vec3 change);

    // move the origin, from which points are
    // rotated & drawn
    Shape3d& operator+= (vec3 change);
    Shape3d& operator-= (vec3 change);

    std::vector<Line3d> lines;
    vec3 origin;
};

Shape3d offset (Shape3d s, vec3 change);

Shape3d operator+ (Shape3d s, vec3 offset);
Shape3d operator- (Shape3d s, vec3 offset);

Shape3d rect (double width, double height);
Shape3d square (double length);
Shape3d box (double width,
             double height,
             double depth);
Shape3d cube (double length);
