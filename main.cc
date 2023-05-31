#include "termvis.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    BrailleCanvas c(TermInfo::detect());
    TextCanvas tc(TermInfo::detect());

    double const len = 50;
    auto cb = cube(len) + vec3 { -len/2, -len/2, 0 };

    /* vec3 const v { 1, 2, 3 }; */
    /* vec<int64_t, 3> const v { 1, 2, 3 }; */
    /* vec<double, 2> const v { 1, 2 }; */
    /* vec<int64_t, 2> const v { 1, 2 };  // broek */
    /* cout << v << endl; */

    tc.write(10, 10, L"hello world.");

    UpdateLoop(60, [&](double t,
                       double dt,
                       int64_t count) {
        (void)t; (void)count;

        c.clear();

        cb.rotate({ 0.5, 1, 1 }, 45 * dt);

        cb.draw(c, 60, 80);

        c.draw();
        tc.draw();

        return false;
    }).start();

    teardown();
}