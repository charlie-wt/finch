#include "finch.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    BrailleCanvas c(TermInfo::detect());
    TextCanvas tc(TermInfo::detect());

    auto cb = Shape::cube(50) + vec3 { 0, 0, 10 };

    auto obj = Mesh::cube(25);

    Cam const cam { 60, 80 };
    (void) cam;

    tc.write(1, 1, L"hello world.");

    UpdateLoop(60, [&](double t,
                       double dt,
                       int64_t count) {
        (void)t; (void)dt; (void)count;

        c.clear();

        cb.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        cb.origin.y() = sin(0.5 * t * M_PI) * 10.;
        cb.draw(c, cam);

        obj.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        obj.origin.y() = sin(0.5 * t * M_PI) * 10.;
        obj.draw(c, cam, DrawMode::FILL);

        c.draw();
        tc.draw();

        return false;
    }).start();

    teardown();
}