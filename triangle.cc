#include "triangle.hh"


Tri& Tri::operator+= (vec3 offset) {
    for (auto &v : verts)
        v += offset;
    return *this;
}
Tri& Tri::operator-= (vec3 offset) {
    for (auto &v : verts)
        v -= offset;
    return *this;
}
Tri& Tri::operator*= (double scale) {
    for (auto &v : verts)
        v *= scale;
    return *this;
}
Tri& Tri::operator/= (double scale) {
    for (auto &v : verts)
        v *= scale;
    return *this;
}

Tri& Tri::operator%= (mat3 const &trans) {
    for (auto &v : verts)
        v = trans % v;
    return *this;
}

Tri operator+ (Tri l, vec3 offset)
    { l += offset; return l; }
Tri operator- (Tri l, vec3 offset)
    { l -= offset; return l; }
Tri operator* (Tri l, double scale)
    { l *= scale; return l; }
Tri operator/ (Tri l, double scale)
    { l /= scale; return l; }
Tri operator% (mat3 const &trans, Tri l)
    { l %= trans; return l; }
