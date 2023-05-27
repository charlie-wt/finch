#include "line.hh"


std::ostream &operator<< (std::ostream &os,
                          Line const &l) {
    os << "(" << l.start << " -> " << l.end << ")";
    return os;
}

Line3d& Line3d::operator+= (vec3 offset) {
    start += offset;
    end += offset;
    return *this;
}
Line3d& Line3d::operator-= (vec3 offset) {
    start -= offset;
    end -= offset;
    return *this;
}
Line3d& Line3d::operator*= (double scale) {
    start *= scale;
    end *= scale;
    return *this;
}
Line3d& Line3d::operator/= (double scale) {
    start /= scale;
    end /= scale;
    return *this;
}

Line3d& Line3d::operator%= (mat3 const &trans) {
    start = trans % start;
    end = trans % end;
    return *this;
}

Line3d operator+ (Line3d l, vec3 offset)
    { l += offset; return l; }
Line3d operator- (Line3d l, vec3 offset)
    { l -= offset; return l; }
Line3d operator* (Line3d l, double scale)
    { l *= scale; return l; }
Line3d operator/ (Line3d l, double scale)
    { l /= scale; return l; }
Line3d operator% (mat3 const &trans, Line3d l)
    { l %= trans; return l; }

std::ostream &operator<< (std::ostream &os,
                          Line3d const &l) {
    os << "(" << l.start << " -> " << l.end << ")";
    return os;
}
