#pragma once

#include "geom.hh"
#include "mesh.hh"

#include <vector>


template <typename Vert>
struct Mesh {
    /* TODO #robustness: sort of assuming Vert
     *                   members atm; do
     *                   transformations more
     *                   properly at some point */
    Mesh<Vert>& rotate (vec3 axis, double degs) {
        auto const trans = rotation(axis, degs);
        for (auto &v : verts) {
            v.pos = trans % v.pos;

            constexpr bool has_norms =
                requires(Vert const &v) { v.norm; };
            if constexpr (has_norms)
                v.norm = trans % v.norm;
        }
        return *this;
    }

    // move all the points, but not the origin
    Mesh<Vert>& offset (vec3 change) {
        for (auto &v : verts) {
            v.pos += change;
        }
        return *this;
    }

    // move the origin, from which points are
    // rotated & drawn
    Mesh<Vert>& operator+= (vec3 change) {
        origin += change;
        return *this;
    }
    Mesh<Vert>& operator-= (vec3 change) {
        origin -= change;
        return *this;
    }

    std::vector<vec3i> indices;
    std::vector<Vert> verts;
    vec3 origin;
};

template <typename V>
Mesh<V> offset (Mesh<V> m, vec3 change)
    { m.offset(change); return m; }

template <typename V>
Mesh<V> operator+ (Mesh<V> m, vec3 change)
    { m += change; return m; }
template <typename V>
Mesh<V> operator- (Mesh<V> m, vec3 change)
    { m -= change; return m; }

struct Pos {
    vec3 pos;
};

struct PosNorm {
    vec3 pos;
    vec3 norm;
};

// shapes
Mesh<PosNorm> box (double width,
                   double height,
                   double depth);

Mesh<PosNorm> cube (double length);

Mesh<PosNorm> sphere (double radius,
                      int64_t subdivisions = 2);
