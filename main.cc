#include "finch.hh"

#include <iostream>

using namespace std;


int main () {
    // vec<float, 3> up { 0, 1, 0 };
    // auto const rot = rotor3f::angle_plane(
    //     -M_PI / 2.f,
    //     bivec3f { 1, 0, 0 });
    // auto const res = rotate(up, rot);
    // (void)res;
    // cout << res << "\n";

    // auto const rot2 = rotor3f::angle_plane(
    //     M_PI / 2.f,
    //     bivec3f { 1, 0, 0 });
    // auto const muld = rot * rot2;
    // cout << muld.scalar << "\n";
    // cout << muld.bivec.xy << "\n";
    // cout << muld.bivec.yz << "\n";
    // cout << muld.bivec.xz << "\n";

    init();

    auto const d = term_dims();
    BrailleCanvas c(d);
    /* CharCanvas c(d); */
    TextCanvas tc(d);

    Framebuffer buf {c.dims};
    auto flat = flat_lit_shader<PosNorm>(buf);
    auto unlit = unlit_shader<PosNorm>(buf);
    (void)unlit;

    auto obj = sphere(c.dims.y() * 0.4, 1);
    auto shell = sphere(c.dims.y() * 0.45, 1);

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

    vec3 cam_base { 0, 0, 50 };
    Cam cam { 60,
              cam_base,
              rotor3f::between_vecs(
                  cam_base.cast<float>(),
                  { 0, 0, 0 })
    };
    (void) cam;

    auto const tmap = bayer(1);

    UpdateLoop(60, [&](double t,
                       double dt,
                       uint64_t count,
                       LoopState const &state) {
        (void)t; (void)dt; (void)count; (void)state;

        tc.clear();
        buf.clear();

        double const amp = 25;
        cam.pos.y() = cam_base.y() + sin(0.5 * t * M_PI) * amp;
        cam.pos.z() = cam_base.z() + cos(0.5 * t * M_PI) * amp;

        obj.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        // obj.origin.y() = sin(0.5 * t * M_PI) * amp;
        flat.draw(obj, cam);

        shell.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        // shell.origin.y() = sin(0.5 * t * M_PI) * amp;
        unlit.draw(shell, cam, DrawMode::LINE);

        tc.write(1, tc.dims.y() - 2,
                 state.speed_report());

        // render_rand(buf.col, c);
        render_threshold_map(buf.col, c, tmap);
        c.draw();
        tc.draw();
        draw_dbg();

        return false;
    }).start();

    teardown();
}