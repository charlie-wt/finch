#include "xmesh.hh"


/* TODO #temp */
XMesh<PosNorm> to_xmesh (Mesh const &m) {
    XMesh<PosNorm> res { m.idxs, {}, m.origin };

    for (size_t i = 0; i < m.verts.size(); i++)
        res.verts.push_back(PosNorm { m.verts[i],
                               m.norms[i] });

    return res;
}

/* XMesh<Pos> to_xmesh (Mesh const &m) { */
/*     XMesh<Pos> res { m.idxs, {}, m.origin }; */

/*     for (size_t i = 0; i < m.verts.size(); i++) */
/*         res.verts.push_back(Pos { m.verts[i] }); */

/*     return res; */
/* } */

// shapes
XMesh<PosNorm> box (double width,
                    double height,
                    double depth) {
    auto const w = width / 2;
    auto const h = height / 2;
    auto const d = depth / 2;
    auto const ns = 1 / vec3 { w, h, d }.length();
    return XMesh<PosNorm> {
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
      { { vec3 { -w, -h,  d },
          vec3 { -w*ns, -h*ns,  d*ns } },
        { vec3 {  w, -h,  d },
          vec3 {  w*ns, -h*ns,  d*ns } },
        { vec3 {  w,  h,  d },
          vec3 {  w*ns,  h*ns,  d*ns } },
        { vec3 { -w,  h,  d },
          vec3 { -w*ns,  h*ns,  d*ns } },
        { vec3 { -w, -h, -d },
          vec3 { -w*ns, -h*ns, -d*ns } },
        { vec3 {  w, -h, -d },
          vec3 {  w*ns, -h*ns, -d*ns } },
        { vec3 {  w,  h, -d },
          vec3 {  w*ns,  h*ns, -d*ns } },
        { vec3 { -w,  h, -d },
          vec3 { -w*ns,  h*ns, -d*ns } } },
        vec3 { 0, 0, 0 }
    };
}

XMesh<PosNorm> cube (double length) {
    return box(length, length, length);
}
