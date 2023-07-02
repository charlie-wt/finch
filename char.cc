#include "char.hh"

#include <iostream>


CharCanvas::CharCanvas (TermInfo const &t)
    : w(t.w)
    , h(t.h)
    , data(w * h, ' ')
    , db(t) {}

void CharCanvas::set (pixel p, char c) {
    if (p.x() < 0 || p.y() < 0 ||
        p.x() >= w || p.y() >= h)
        return;
    data[(p.y() * w) + p.x()] = c;
}

void CharCanvas::set (pixel p, double depth) {
    if (!db.set(p, depth))
        return;

    int const zi = depth;
    char const ch = zi < 0
        ? (depth == db.far ? '.' : 'v')
        : zi > 9
            ? '^'
            : std::to_string(zi)[0];

    set(p, ch);
}

void CharCanvas::draw () const {
    std::cout << "\r" << data << std::flush;
}

void CharCanvas::clear () {
    data = std::string(w * h, ' ');
    db.clear();
}
