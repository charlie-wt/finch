#include "termvis.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    BrailleCanvas c(TermInfo::detect());

    auto cb = cube(50) + vec3 { 30, 30, 0 };

    UpdateLoop(60, [&](double t,
                       double dt,
                       int64_t count) {
        c.clear();

        cb.rotate({ 0, 1, 1 }, 20 * dt);

        cb.draw(c, 60, 80);

        c.draw();

        return false;
    }).start();

    teardown();
}