#pragma once

#include "line.hh"

#include <vector>


struct Shape3d {
    template<typename Canvas>
    void draw (Canvas &canvas) const {
        for (auto const &l : lines)
            l.draw(canvas);
    }

    void rotate (Point3d axis, double rads);

    Shape3d& operator+= (Point3d offset);
    Shape3d& operator-= (Point3d offset);

    std::vector<Line3d> lines;
    Point3d origin;
};

Shape3d operator+ (Shape3d s, Point3d offset);
Shape3d operator- (Shape3d s, Point3d offset);

Shape3d rect (double width, double height);
Shape3d square (double length);
Shape3d box (double width,
             double height,
             double depth);
Shape3d cube (double length);
