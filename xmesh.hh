#pragma once

#include "geom.hh"
#include "mesh.hh"

/* #include <unordered_map> */
#include <vector>


/* enum class VertexDataType { */
/*     POSITION, */
/*     NORMAL, */
/*     COLOUR */
/* }; */
/* struct XMesh { */
/*     std::vector<vec3i> indices; */
/*     std::unordered_map<VertexDataType, vec3> verts; */
/*     vec3 origin; */
/* }; */

template <typename Vert>
struct XMesh {
    /* TODO #robustness: assuming Vert members
     *                   atm; do transformations
     *                   more properly at some
     *                   point */
    XMesh<Vert>& rotate(vec3 axis, double degs) {
        auto const trans = rotation(axis, degs);
        for (auto &v : verts) {
            v.pos = trans % v.pos;
            v.norm = trans % v.norm;
        }
        return *this;
    }

    // move all the points, but not the origin
    XMesh<Vert>& offset(vec3 change) {
        for (auto &v : verts) {
            v.pos += change;
        }
        return *this;
    }

    // move the origin, from which points are
    // rotated & drawn
    XMesh<Vert>& operator+=(vec3 change) {
        origin += change;
        return *this;
    }
    XMesh<Vert>& operator-=(vec3 change) {
        origin -= change;
        return *this;
    }

    std::vector<vec3i> indices;
    std::vector<Vert> verts;
    vec3 origin;
};

template <typename V>
XMesh<V> offset(XMesh<V> m, vec3 change)
    { m.offset(change); return m; }

template <typename V>
XMesh<V> operator+(XMesh<V> m, vec3 change)
    { m += change; return m; }
template <typename V>
XMesh<V> operator-(XMesh<V> m, vec3 change)
    { m -= change; return m; }

struct Pos {
    vec3 pos;
};

struct PosNorm {
    vec3 pos;
    vec3 norm;
};

XMesh<PosNorm> to_xmesh (Mesh const &m);
