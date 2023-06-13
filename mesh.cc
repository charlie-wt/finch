#include "mesh.hh"

#include "util.hh"


// factories
Mesh Mesh::from_verts (
    std::vector<vec3> const &verts) {
    std::vector<vec3i> idxs;
    int64_t const num_tris = verts.size() / 3;
    idxs.reserve(num_tris);
    for (int64_t i = 0; i < num_tris; i++)
        idxs.push_back({ 3*i,
                         3*i + 1,
                         3*i + 2 });
    return Mesh
        { verts, { }, idxs, vec3::zeros() };
}

Mesh Mesh::from_tris (
    std::vector<Tri> const &tris) {
    std::vector<vec3> verts;
    verts.reserve(tris.size() * 3);

    for (auto const &t : tris) {
        verts.push_back(t.verts[0]);
        verts.push_back(t.verts[1]);
        verts.push_back(t.verts[2]);
    }

    return Mesh::from_verts(verts);
}

// shapes
Mesh Mesh::box(double width,
               double height,
               double depth) {
    auto const w = width / 2;
    auto const h = height / 2;
    auto const d = depth / 2;
    return Mesh {
        { vec3 { -w, -h,  d },
          vec3 {  w, -h,  d },
          vec3 {  w,  h,  d },
          vec3 { -w,  h,  d },
          vec3 { -w, -h, -d },
          vec3 {  w, -h, -d },
          vec3 {  w,  h, -d },
          vec3 { -w,  h, -d } },
        { /* TODO #finish */ },
        { // front
          vec3i { 0, 1, 2 },
          vec3i { 2, 3, 0 },
          // back
          vec3i { 4, 5, 6 },
          vec3i { 6, 7, 4 },
          // left
          vec3i { 0, 4, 7 },
          vec3i { 7, 3, 0 },
          // right
          vec3i { 1, 5, 6 },
          vec3i { 6, 2, 1 },
          // top
          vec3i { 3, 7, 6 },
          vec3i { 6, 2, 3 },
          // bottom
          vec3i { 0, 4, 5 },
          vec3i { 5, 1, 0 } },
        vec3 { 0, 0, 0 }
    };
}

Mesh Mesh::cube(double length) {
    return Mesh::box(length, length, length);
}

// arith
Mesh& Mesh::rotate (vec3 axis, double degs) {
    auto const trans = rotation(axis, degs);
    for (auto &v : verts)
        v = trans % v;
    return *this;
}

Mesh& Mesh::offset (vec3 change) {
    for (auto &v : verts)
        v += change;
    return *this;
}

Mesh& Mesh::operator+= (vec3 change) {
    origin += change;
    return *this;
}
Mesh& Mesh::operator-= (vec3 change) {
    origin -= change;
    return *this;
}

Mesh offset (Mesh s, vec3 change)
    { s.offset(change); return s; }

Mesh operator+ (Mesh s, vec3 change)
    { s += change; return s; }
Mesh operator- (Mesh s, vec3 change)
    { s -= change; return s; }

