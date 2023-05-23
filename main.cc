#include "termvis.hh"

#include <iostream>

using namespace std;


int main () {
    /* CharCanvas c(TermInfo::detect()); */
    BrailleCanvas c(TermInfo::detect());

    vector<Line> lines;

    /* lines.push_back({ { 0., 0. }, { 50., 50. } }); */
    /* lines.push_back({ { 50., 50. }, { 0., 0. } }); */
    /* lines.push_back({ { 0., 50. }, { 50., 0. } }); */
    /* lines.push_back({ { 50., 0. }, { 0., 50. } }); */
    /* lines.push_back({ { 0., 0. }, { 0., 50. } }); */
    /* lines.push_back({ { 0., 0. }, { 50., 0. } }); */

    double const midx = c.w/2;
    double const midy = c.h/2;
    double const rad = 30.;
    lines.push_back({ { midx, midy - rad },
                      { midx, midy + rad } });

    auto const sq = square(50) + Point3d { 30, 30 };

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

        /* Point { midx + xoff, midy - yoff }.draw(c); */
        /* Point { midx - xoff, midy + yoff }.draw(c); */

        for (auto const &l : lines)
            l.draw(c);

        sq.draw(c);

        c.draw();

        return false;
    }).start();

    cout << "\n";
}