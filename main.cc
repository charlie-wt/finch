#include "termvis.hh"

#include <iostream>

using namespace std;


int main () {
    /* CharCanvas cg(TermInfo::detect()); */
    BrailleCanvas cg(TermInfo::detect());

    vector<Line> lines;
    lines.push_back({ { 0., 0. }, { 50., 5. } });
    lines.push_back({ { 0., 0. }, { 5., 20. } });
    lines.push_back({ { 0., 0. }, { 20., 20. } });
    lines.push_back({ { 40., 20. }, { 10., 13. } });

    UpdateLoop(4, [&](double t,
                      double dt,
                      int64_t count) {
        cg.clear();

        lines[0].end.y += 4 * dt;
        for (auto const &l : lines)
            l.draw(cg);

        cg.draw();

        return false;
    }).start();

    cout << "\n";
}