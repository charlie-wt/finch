#include "finch.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    BrailleCanvas c(TermInfo::detect());
    /* CharCanvas c(TermInfo::detect()); */
    TextCanvas tc(TermInfo::detect());

    auto cb = Shape::cube(c.h * 0.5);

    auto obj = Mesh::cube(c.h * 0.4);

    /* double const w = c.w * 0.5; */
    /* double const h = c.h * 0.5; */
    /* auto obj = Mesh { */
    /*     { vec3 { -w/2, -h/2, 0 }, */
    /*       vec3 {    0,  h/2, 0 }, */
    /*       vec3 {  w/2, -h/2, 0 } }, */
    /*     { }, */
    /*     { vec3i { 0, 1, 2 } }, */
    /*     vec3 { 0, 0, 0 } */
    /* }; */
    /* obj.rotate({ 0, 0, 1 }, 90); */
    /* obj.rotate({ 0, 1, 0 }, -89); */

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