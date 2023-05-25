#include "termvis.hh"

#include <iostream>

using namespace std;


int main () {
    BrailleCanvas c(TermInfo::detect());

    vector<Line> lines;

    double const midx = c.w/2;
    double const midy = c.h/2;
    double const rad = 30.;
    lines.push_back({ { midx, midy - rad },
                      { midx, midy + rad } });

    auto const cb = cube(50) + Point3d { 30, 30, 0 };

    auto const m = mat4::eye() * 2;
    cout << m << "\n";
    auto const m2 = matf<4, 2>::ones();
    cout << m2 << "\n";
    cout << m % m2 << "\n";

    UpdateLoop(4, [&](double t,
                      double dt,
                      int64_t count) {
        c.clear();

        double const xoff = rad * sin(t);
        double const yoff = rad * cos(t);
        lines[0].start.x = midx + xoff;
        lines[0].end.x = midx - xoff;
        lines[0].start.y = midy - yoff;
        lines[0].end.y = midy + yoff;

        for (auto const &l : lines)
            l.draw(c);

        cb.draw(c);

        c.draw();

        return false;
    });//.start();

    cout << "\n";
}