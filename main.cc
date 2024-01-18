#include "finch.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    auto const ti = TermInfo::detect();
    BrailleCanvas c(ti);
    /* CharCanvas c(ti); */
    TextCanvas tc(ti);

    auto sh = flat_lit_shader<PosNorm>(ti);

    /* auto cb = Shape::cube(c.h * 0.5); */
    /* auto ubj = cube(c.h * 0.4); */

    /* XMesh<Pos> obj { ubj.indices, {}, ubj.origin }; */
    /* obj.verts.reserve(ubj.verts.size()); */
    /* for (auto const & v : ubj.verts) */
    /*     obj.verts.emplace_back(v.pos); */

    auto obj = sphere(c.h * 0.4, 0);
    auto shell = sphere(c.h * 0.45, 0);

    /* double mag = 50; */
    /* double inc = 1; */
    /* sh.depth_buf.near = -mag; */

    /* double const w = c.w * 0.75; */
    /* double const h = c.h * 0.75; */
    /* auto obj = Mesh { */
    /*     { vec3 { -w/2, -h/2, 0 }, */
    /*       vec3 {    0,  h/2, 0 }, */
    /*       vec3 {  w/2, -h/2, 0 } }, */
    /*     { }, */
    /*     { vec3i { 0, 1, 2 } }, */
    /*     vec3 { 0, 0, 0 } */
    /* }; */
    /* obj.rotate({ 0, 0, 1 }, 90); */

    /* double const w = c.w * 0.75; */
    /* double const h = c.h * 0.75; */
    /* auto obj = XMesh<PosNorm> { */
    /*     { vec3i { 0, 1, 2 } }, */
    /*     { { vec3 { -w/2, -h/2, 0 }, */
    /*         vec3 {    0,    0, 1 } }, */
    /*       { vec3 {    0,  h/2, 0 } , */
    /*         vec3 {    0,    0, 1 } }, */
    /*       { vec3 {  w/2, -h/2, 0 } , */
    /*         vec3 {    0,    0, 1 } } }, */
    /*     vec3 { 0, 0, 0 } */
    /* }; */
    /* /1* obj.rotate({ 0, 0, 1 }, 90); *1/ */

    Cam const cam { 60, 80 };
    (void) cam;

    UpdateLoop(60, [&](double t,
                       double dt,
                       uint64_t count,
                       LoopState const &state) {
        (void)t; (void)dt; (void)count; (void)state;

        c.clear();
        tc.clear();
        sh.clear();

        /* cb.rotate({ 0.5, 1, 0.75 }, 60 * dt); */
        /* cb.origin.y() = sin(0.5 * t * M_PI) * 10.; */
        /* cb.draw(c, cam); */

        obj.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        obj.origin.y() = sin(0.5 * t * M_PI) * 10.;
        /* obj.draw(c, cam, DrawMode::FILL); */
        sh.draw(obj, cam);

        shell.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        shell.origin.y() = sin(0.5 * t * M_PI) * 10.;
        /* shell.draw(c, cam, DrawMode::LINE); */
        sh.draw(shell, cam, DrawMode::LINE);

        tc.write(1, tc.h - 2, state.speed_report());

        sh.render_to(c);
        /* cb.draw(c, cam); */
        c.draw();
        tc.draw();
        draw_dbg();

        /* if (sh.depth_buf.near <= -mag) */
        /*     inc = 1; */
        /* else if (sh.depth_buf.near >= mag) */
        /*     inc = -1; */
        /* sh.depth_buf.near += inc; */

        return false;
    }).start();

    teardown();
}