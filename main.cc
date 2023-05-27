#include "termvis.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    BrailleCanvas c(TermInfo::detect());

    double len = 50;
    auto cb = cube(len) + vec3 { -len/2, -len/2, 0 };

    UpdateLoop(60, [&](double t,
                       double dt,
                       int64_t count) {
        c.clear();

        cb.rotate({ 0.5, 1, 1 }, 45 * dt);

        cb.draw(c, 60, 80);

        c.draw();

        return false;
    }).start();

    teardown();
}