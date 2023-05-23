#include "line.hh"


Line3d& Line3d::operator+= (Point3d offset) {
    start += offset;
    end += offset;
    return *this;
}
Line3d& Line3d::operator-= (Point3d offset) {
    start -= offset;
    end -= offset;
    return *this;
}
Line3d& Line3d::operator*= (double scale) {
    start *= scale;
    end *= scale;
    return *this;
}
Line3d& Line3d::operator/= (double scale) {
    start /= scale;
    end /= scale;
    return *this;
}

Line3d operator+ (Line3d l, Point3d offset) {
    l += offset; return l;
}
Line3d operator- (Line3d l, Point3d offset) {
    l -= offset; return l;
}
Line3d operator* (Line3d l, double scale) {
    l *= scale; return l;
}
Line3d operator/ (Line3d l, double scale) {
    l /= scale; return l;
}
