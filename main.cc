#include "termvis.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    BrailleCanvas c(TermInfo::detect());
    TextCanvas tc(TermInfo::detect());

    double const len = 50;
    auto cb = cube(len) + vec3 { 0, 0, 10 };

    Mesh const tr { { Tri { vec3 { -25, -25, 0 },
                            vec3 {   0,  25, 0 },
                            vec3 {  25, -25, 0 } } } };

    Cam const cam { 60, 80 };

    /* vec3 const v { 1, 2, 3 }; */
    /* vec<int64_t, 3> const v { 1, 2, 3 }; */
    /* vec<double, 2> const v { 1, 2 }; */
    /* vec<int64_t, 2> const v { 1, 2 };  // broek */
    /* cout << v << endl; */

    tc.write(1, 1, L"hello world.");

    UpdateLoop(60, [&](double t,
                       double dt,
                       int64_t count) {
        (void)t; (void)dt; (void)count;

        c.clear();

        /* cb.rotate({ 0.5, 1, 0.75 }, 60 * dt); */
        /* cb.origin.y() = sin(0.5 * t * M_PI) * 10.; */
        /* cb.draw(c, cam); */

        tr.draw(c, cam, DrawMode::LINE);

        c.draw();
        tc.draw();

        return false;
    }).start();

    teardown();
}