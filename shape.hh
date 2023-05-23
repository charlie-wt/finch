#pragma once

#include "line.hh"

#include <vector>


struct Shape3d {
    template<typename Canvas>
    void draw (Canvas &canvas) const {
        for (auto const &l : lines)
            l.draw(canvas);
    }

    Shape3d& operator+= (Point3d offset);
    Shape3d& operator-= (Point3d offset);
    Shape3d& operator*= (double scale);
    Shape3d& operator/= (double scale);

    std::vector<Line3d> lines;
};

Shape3d operator+ (Shape3d s, Point3d offset);
Shape3d operator- (Shape3d s, Point3d offset);
Shape3d operator* (Shape3d s, double scale);
Shape3d operator/ (Shape3d s, double scale);

Shape3d rect (double width, double height);
Shape3d square (double length);
Shape3d box (double width, double height);
