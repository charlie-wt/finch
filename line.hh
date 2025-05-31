#pragma once

#include "camera.hh"
#include "draw.hh"
#include "geom.hh"


struct Line {
    vec2 start, end;
};

std::ostream &operator<< (std::ostream &os,
                          Line const &l);

struct Line3d {
    template<typename Canvas>
    void draw (Canvas &canvas,
               Cam const &cam) const {
        draw_line(start, end, canvas, cam);
    }

    Line flatten () const {
        return { start.to<vec2>(), end.to<vec2>() };
    }

    template<typename Canvas>
    Line project (Canvas &canvas,
                  Cam const &cam) const {
        return Line3d {
            cam.projected(start, canvas),
            cam.projected(end, canvas)
        }.flatten();
    }

    Line3d& operator+= (vec3 offset);
    Line3d& operator-= (vec3 offset);
    Line3d& operator*= (double scale);
    Line3d& operator/= (double scale);
    Line3d& operator%= (mat3 const &trans);

    vec3 start, end;
};

Line3d operator+ (Line3d l, vec3 offset);
Line3d operator- (Line3d l, vec3 offset);
Line3d operator* (Line3d l, double scale);
Line3d operator/ (Line3d l, double scale);
Line3d operator% (mat3 const &trans, Line3d l);

std::ostream &operator<< (std::ostream &os,
                          Line3d const &l);
