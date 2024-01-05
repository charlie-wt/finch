#include "geom.hh"


vec3 barycentric (vec3 p,
                  vec3 tri1, vec3 tri2, vec3 tri3) {
    double const det = (tri2.y() - tri3.y()) *
                       (tri1.x() - tri3.x()) +
                       (tri3.x() - tri2.x()) *
                       (tri1.y() - tri3.y());

    if (abs(det) < 1e-3) return { -1, 1, 1 };

    double const l1 = ((tri2.y() - tri3.y()) *
                       (   p.x() - tri3.x()) +
                       (tri3.x() - tri2.x()) *
                       (   p.y() - tri3.y())) / det;
    double const l2 = ((tri3.y() - tri1.y()) *
                       (   p.x() - tri3.x()) +
                       (tri1.x() - tri3.x()) *
                       (   p.y() - tri3.y())) / det;

    return { l1, l2, 1.0 - l1 - l2 };
}
