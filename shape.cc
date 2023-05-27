#include "shape.hh"


void Shape3d::rotate (vec3 axis, double degs) {
    auto const trans = rotation(axis, degs);

    for (auto &l : lines) {
        l -= origin;
        l %= trans;
        l += origin;
    }
}

Shape3d& Shape3d::operator+= (vec3 offset) {
    origin += offset;
    for (auto &l : lines)
        l += offset;
    return *this;
}
Shape3d& Shape3d::operator-= (vec3 offset) {
    origin -= offset;
    for (auto &l : lines)
        l -= offset;
    return *this;
}

Shape3d operator+ (Shape3d s, vec3 offset)
    { s += offset; return s; }
Shape3d operator- (Shape3d s, vec3 offset)
    { s -= offset; return s; }

Shape3d rect (double width, double height) {
    return { {
        { { 0., 0., 0. }, { width, 0., 0. } },
        { { width, 0., 0. }, { width, height, 0. } },
        { { width, height, 0. }, { 0., height, 0. } },
        { { 0., height, 0. }, { 0., 0., 0. } }
    }, { width / 2., height / 2., 0. } };
}

Shape3d square (double length)
    { return rect(length, length); }

Shape3d box (double width,
             double height,
             double depth) {
    Shape3d back = rect(width, height);
    Shape3d front = back + vec3 { 0., 0., depth };
    std::vector<Line3d> all_lines {
        { { 0., 0., 0. }, { 0., 0., depth } },
        { { width, 0., 0. }, { width, 0., depth } },
        { { width, height, 0. }, { width, height, depth } },
        { { 0., height, 0. }, { 0., height, depth } },
    };

    all_lines.insert(all_lines.end(),
                     back.lines.begin(),
                     back.lines.end());
    all_lines.insert(all_lines.end(),
                     front.lines.begin(),
                     front.lines.end());

    return { all_lines, { width / 2,
                          height / 2,
                          depth / 2 } };
}

Shape3d cube (double length)
    { return box(length, length, length); }
