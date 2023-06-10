#include "mesh.hh"


Mesh& Mesh::rotate (vec3 axis, double degs) {
    auto const trans = rotation(axis, degs);
    for (auto &t : tris)
        t %= trans;
    return *this;
}

Mesh& Mesh::offset (vec3 change) {
    for (auto &t : tris)
        t += change;
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

