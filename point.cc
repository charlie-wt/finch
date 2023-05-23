#include "point.hh"


std::ostream &operator<< (std::ostream &os, Pixel p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

Pixel Point::to_pixel () const {
    return { static_cast<int64_t>(x),
             static_cast<int64_t>(y) };
}

Pixel Point3d::to_pixel () const {
    return perspective().to_pixel();
}

Point Point3d::perspective () const {
    /* TODO #finish */
    return { x, y };
}

Point3d& Point3d::operator+= (double offset) {
    x += offset;
    y += offset;
    z += offset;
    return *this;
}
Point3d& Point3d::operator+= (Point3d other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
Point3d& Point3d::operator-= (double offset) {
    x -= offset;
    y -= offset;
    z -= offset;
    return *this;
}
Point3d& Point3d::operator-= (Point3d other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}
Point3d& Point3d::operator*= (double scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}
Point3d& Point3d::operator*= (Point3d other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}
Point3d& Point3d::operator/= (double scale) {
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
}
Point3d& Point3d::operator/= (Point3d other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

Point3d operator+ (Point3d p, double offset) {
    p += offset; return p;
}
Point3d operator+ (Point3d p, Point3d other) {
    p += other; return p;
}
Point3d operator- (Point3d p, double offset) {
    p -= offset; return p;
}
Point3d operator- (Point3d p, Point3d other) {
    p -= other; return p;
}
Point3d operator* (Point3d p, double scale) {
    p *= scale; return p;
}
Point3d operator* (Point3d p, Point3d other) {
    p *= other; return p;
}
Point3d operator/ (Point3d p, double scale) {
    p /= scale; return p;
}
Point3d operator/ (Point3d p, Point3d other) {
    p /= other; return p;
}
