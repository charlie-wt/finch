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
