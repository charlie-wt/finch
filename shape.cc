#include "shape.hh"


Shape3d& Shape3d::operator+= (Point3d offset) {
    for (auto &l : lines)
        l += offset;
    return *this;
}
Shape3d& Shape3d::operator-= (Point3d offset) {
    for (auto &l : lines)
        l -= offset;
    return *this;
}
Shape3d& Shape3d::operator*= (double scale) {
    for (auto &l : lines)
        l *= scale;
    return *this;
}
Shape3d& Shape3d::operator/= (double scale) {
    for (auto &l : lines)
        l /= scale;
    return *this;
}

Shape3d operator+ (Shape3d s, Point3d offset) {
    s += offset; return s;
}
Shape3d operator- (Shape3d s, Point3d offset) {
    s -= offset; return s;
}
Shape3d operator* (Shape3d s, double scale) {
    s *= scale; return s;
}
Shape3d operator/ (Shape3d s, double scale) {
    s /= scale; return s;
}

Shape3d rect (double width, double height) {
    return { {
        { { 0., 0., 0. }, { width, 0., 0. } },
        { { width, 0., 0. }, { width, height, 0. } },
        { { width, height, 0. }, { 0., height, 0. } },
        { { 0., height, 0. }, { 0., 0., 0. } }
    } };
}

Shape3d square (double length) {
    return rect(length, length);
}

Shape3d box (double width, double height) {
    return { {
        { { 0., 0., 0. }, { width, 0., 0. } },
        { { width, 0., 0. }, { width, height, 0. } },
        { { width, height, 0. }, { 0., height, 0. } },
        { { 0., height, 0. }, { 0., 0., 0. } }
    } };
}
