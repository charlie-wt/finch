#pragma once

#include <iostream>


struct Pixel {
    template<typename Canvas>
    void draw (Canvas &canvas) const {
        canvas.set(*this);
    }

    int64_t x, y;
};

std::ostream &operator<< (std::ostream &os, Pixel p);

/* TODO #enhancement: more proper 2d/3d geom lib */
struct Point {
    Pixel to_pixel () const;

    template<typename Canvas>
    void draw (Canvas &canvas) const {
        canvas.set(to_pixel());
    }

    double x, y;
};

struct Point3d {
    Pixel to_pixel () const;

    Point perspective () const;

    template<typename Canvas>
    void draw (Canvas &canvas) const {
        canvas.set(to_pixel());
    }

    Point3d& operator+= (double offset);
    Point3d& operator+= (Point3d other);
    Point3d& operator-= (double offset);
    Point3d& operator-= (Point3d other);
    Point3d& operator*= (double scale);
    Point3d& operator*= (Point3d other);
    Point3d& operator/= (double scale);
    Point3d& operator/= (Point3d other);

    double x, y, z;
};

Point3d operator+ (Point3d p, double offset);
Point3d operator+ (Point3d p, Point3d other);
Point3d operator- (Point3d p, double offset);
Point3d operator- (Point3d p, Point3d other);
Point3d operator* (Point3d p, double scale);
Point3d operator* (Point3d p, Point3d other);
Point3d operator/ (Point3d p, double scale);
Point3d operator/ (Point3d p, Point3d other);
