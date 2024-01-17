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

namespace {

XMesh<PosNorm> icosahedron (double radius) {
    const float phi = (1.0f + sqrt(5.0f)) / 2.0f;
    return {
        { // Create triangles, using indices
            {  0, 11,  5 },
            {  0,  5,  1 },
            {  0,  1,  7 },
            {  0,  7, 10 },
            {  0, 10, 11 },

            {  1,  5,  9 },
            {  5, 11,  4 },
            { 11, 10,  2 },
            { 10,  7,  6 },
            {  7,  1,  8 },

            {  3,  9,  4 },
            {  3,  4,  2 },
            {  3,  2,  6 },
            {  3,  6,  8 },
            {  3,  8,  9 },

            {  4,  9,  5 },
            {  2,  4, 11 },
            {  6,  2, 10 },
            {  8,  6,  7 },
            {  9,  8,  1 }
        },
        { // create vertices - from three
          // orthogonal golden rectangles
            //              x     y     z
            { norm(vec3 {  -1,  phi,    0 }, radius),
              norm(vec3 {  -1,  phi,    0 }) },
            { norm(vec3 {   1,  phi,    0 }, radius),
              norm(vec3 {   1,  phi,    0 }) },
            { norm(vec3 {  -1, -phi,    0 }, radius),
              norm(vec3 {  -1, -phi,    0 }) },
            { norm(vec3 {   1, -phi,    0 }, radius),
              norm(vec3 {   1, -phi,    0 }) },

            { norm(vec3 {   0,   -1,  phi }, radius),
              norm(vec3 {   0,   -1,  phi }) },
            { norm(vec3 {   0,    1,  phi }, radius),
              norm(vec3 {   0,    1,  phi }) },
            { norm(vec3 {   0,   -1, -phi }, radius),
              norm(vec3 {   0,   -1, -phi }) },
            { norm(vec3 {   0,    1, -phi }, radius),
              norm(vec3 {   0,    1, -phi }) },

            { norm(vec3 { phi,    0,   -1 }, radius),
              norm(vec3 { phi,    0,   -1 }) },
            { norm(vec3 { phi,    0,    1 }, radius),
              norm(vec3 { phi,    0,    1 }) },
            { norm(vec3 { -phi,    0,   -1 }, radius),
              norm(vec3 { -phi,    0,   -1 }) },
            { norm(vec3 { -phi,    0,    1 }, radius),
              norm(vec3 { -phi,    0,    1 }) }
        },
        vec3 {0, 0, 0}
    };
}

XMesh<PosNorm> subdivide (XMesh<PosNorm> const &mesh,
                          double radius) {
    auto res = mesh;

    for (auto const &idx : mesh.indices) {
        // get the coordinates of the current
        // triangle.
        auto const vert1 = mesh.verts[idx[0]].pos;
        auto const vert2 = mesh.verts[idx[1]].pos;
        auto const vert3 = mesh.verts[idx[2]].pos;

        // find the (normalised) midpoints of each
        // edge.
        auto const mid1 = norm((vert1 + vert2) * 0.5);
        auto const mid2 = norm((vert2 + vert3) * 0.5);
        auto const mid3 = norm((vert3 + vert1) * 0.5);

        // get the indices of the vertices to add.
        auto const mid1_idx = static_cast<int64_t>(res.verts.size());
        auto const mid2_idx = mid1_idx + 1;
        auto const mid3_idx = mid2_idx + 1;

        // add the new vertices.
        res.verts.emplace_back(norm(mid1, radius),
                               mid1);
        res.verts.emplace_back(norm(mid2, radius),
                               mid2);
        res.verts.emplace_back(norm(mid3, radius),
                               mid3);

        // add new indices, to specify the new
        // triangles.
        res.indices.push_back(vec3i {
            idx[0], mid1_idx, mid3_idx });
        res.indices.push_back(vec3i {
            mid1_idx, idx[1], mid2_idx });
        res.indices.push_back(vec3i {
            mid2_idx, idx[2], mid3_idx });
        res.indices.push_back(vec3i {
            mid1_idx, mid2_idx, mid3_idx });
    }
    return res;
}

}

XMesh<PosNorm> sphere (double radius,
                       int64_t subdivisions) {
    XMesh<PosNorm> current_mesh = icosahedron(radius);

    for (int64_t i = 0; i < subdivisions; i++)
        current_mesh = subdivide(current_mesh, radius);

    return current_mesh;
}
