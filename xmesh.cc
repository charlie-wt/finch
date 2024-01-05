#include "xmesh.hh"


XMesh<PosNorm> to_xmesh (Mesh const &m) {
    XMesh<PosNorm> res { m.idxs, {}, m.origin };

    for (size_t i = 0; i < m.verts.size(); i++)
        res.verts.push_back(PosNorm { m.verts[i],
                               m.norms[i] });

    return res;
}
