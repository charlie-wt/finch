#pragma once

#include "camera.hh"
#include "line.hh"

#include <vector>


struct Shape {
    static Shape rect (double width,
                       double height);
    static Shape square (double length);
    static Shape box (double width,
                      double height,
                      double depth);
    static Shape cube (double length);

    template<typename Canvas>
    void draw (Canvas &canvas,
               Cam const &cam) const {
        for (auto l : lines) {
            l += origin;
            l.draw(canvas, cam);
        }
    }

    Shape& rotate (vec3 axis, double degs);

    // move all the points, but not the origin
    Shape& offset (vec3 change);

    // move the origin, from which points are
    // rotated & drawn
    Shape& operator+= (vec3 change);
    Shape& operator-= (vec3 change);

    std::vector<Line3d> lines;
    vec3 origin;
};

Shape offset (Shape s, vec3 change);

Shape operator+ (Shape s, vec3 offset);
Shape operator- (Shape s, vec3 offset);
