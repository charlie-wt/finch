#include "finch.hh"

#include <iostream>

using namespace std;


int main () {
    init();

    auto const d = term_dims();
    BrailleCanvas c(d);
    /* CharCanvas c(d); */
    TextCanvas tc(d);

    Framebuffer buf {c.dims};
    auto flat = flat_lit_shader<PosNorm>(buf);
    auto unlit = unlit_shader<PosNorm>(buf);

    /* auto cb = Shape::cube(c.dims.y() * 0.5); */

    auto obj = sphere(c.dims.y() * 0.4, 0);
    auto shell = sphere(c.dims.y() * 0.45, 0);

    /* double mag = 75; */
    /* double inc = 1; */
    /* buf.depth.near = -mag; */

    /* double const w = c.dims.x() * 0.75; */
    /* double const h = c.dims.y() * 0.75; */
    /* auto gobj = Mesh<PosNorm> { */
    /*     { vec3i { 0, 1, 2 } }, */
    /*     { { vec3 { -w/2, -h/2, 0 }, */
    /*         vec3 {    0,    0, 1 } }, */
    /*       { vec3 {    0,  h/2, 0 } , */
    /*         vec3 {    0,    0, 1 } }, */
    /*       { vec3 {  w/2, -h/2, 0 } , */
    /*         vec3 {    0,    0, 1 } } }, */
    /*     vec3 { 0, 0, 0 } */
    /* }; */
    /* auto obj = coloured(gobj, red); */
    /* obj.rotate({ 0, 0, 1 }, 90); */

    Cam const cam { 60, 80 };
    (void) cam;

    auto const tmap = bayer(2);

    UpdateLoop(60, [&](double t,
                       double dt,
                       uint64_t count,
                       LoopState const &state) {
        (void)t; (void)dt; (void)count; (void)state;

        /* c.clear(); */
        tc.clear();
        buf.clear();

        /* cb.rotate({ 0.5, 1, 0.75 }, 60 * dt); */
        /* cb.origin.y() = sin(0.5 * t * M_PI) * 10.; */
        /* cb.draw(c, cam); */

        double const amp = 25;

        obj.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        obj.origin.y() = sin(0.5 * t * M_PI) * amp;
        flat.draw(obj, cam);

        shell.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        shell.origin.y() = sin(0.5 * t * M_PI) * amp;
        unlit.draw(shell, cam, DrawMode::LINE);

        tc.write(1, tc.dims.y() - 2,
                 state.speed_report());

        // render_rand(buf.col, c);
        render_threshold_map(buf.col, c, tmap);
        c.draw();
        tc.draw();
        draw_dbg();

        /* if (buf.depth.near <= -mag) */
        /*     inc = 1; */
        /* else if (buf.depth.near >= mag) */
        /*     inc = -1; */
        /* buf.depth_buf.near += inc; */

        return false;
    }).start();

    teardown();
}