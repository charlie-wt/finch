#include "shape.hh"


Shape& Shape::rotate (vec3 axis, double degs) {
    auto const trans = rotation(axis, degs);
    for (auto &l : lines)
        l %= trans;
    return *this;
}

Shape& Shape::offset (vec3 change) {
    for (auto &l : lines)
        l += change;
    return *this;
}

Shape& Shape::operator+= (vec3 change) {
    origin += change;
    return *this;
}
Shape& Shape::operator-= (vec3 change) {
    origin -= change;
    return *this;
}

Shape offset (Shape s, vec3 change)
    { s.offset(change); return s; }

Shape operator+ (Shape s, vec3 change)
    { s += change; return s; }
Shape operator- (Shape s, vec3 change)
    { s -= change; return s; }

Shape rect (double width, double height) {
    double const left = -width/2;
    double const right = width/2;
    double const top = height/2;
    double const bottom = -height/2;
    return { {
        { { left, bottom, 0. }, { right, bottom, 0. } },
        { { right, bottom, 0. }, { right, top, 0. } },
        { { right, top, 0. }, { left, top, 0. } },
        { { left, top, 0. }, { left, bottom, 0. } }
    }, { 0., 0., 0. } };
}

Shape square (double length)
    { return rect(length, length); }

Shape box (double width,
           double height,
           double depth) {
    double const fr = depth/2;
    double const bk = -depth/2;

    Shape back = rect(width, height).offset(
        vec3 { 0., 0., bk });
    Shape front = offset(back, vec3 { 0., 0., depth });

    double const left = -width/2;
    double const right = width/2;
    double const top = height/2;
    double const bottom = -height/2;

    std::vector<Line3d> all_lines {
        { { left, bottom, bk }, { left, bottom, fr } },
        { { right, bottom, bk }, { right, bottom, fr } },
        { { right, top, bk }, { right, top, fr } },
        { { left, top, bk }, { left, top, fr } },
    };

    all_lines.insert(all_lines.end(),
                     back.lines.begin(),
                     back.lines.end());
    all_lines.insert(all_lines.end(),
                     front.lines.begin(),
                     front.lines.end());

    return { all_lines, { 0., 0., 0. } };
}

Shape cube (double length)
    { return box(length, length, length); }
