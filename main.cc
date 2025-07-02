#include "finch.hh"

#include <iostream>

using namespace std;


int main () {

    // mat4 const ok {
    //     vec4 {1, 0, 0, 0},
    //     vec4 {0, 1, 0, 0},
    //     vec4 {0, 0, 1, 0},
    //     vec4 {0, 0, 0, 1}
    // };
    // cout << pr(ok) << "\n";
    // return 0;

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
    // auto unlit = unlit_shader<PosNorm>(buf);
    // (void)unlit;

    auto const radius = c.dims.y() * 0.4; (void)radius;
    auto obj = sphere(radius, 0);
    // auto obj = cube(radius * 2);
    // auto shell = sphere(c.dims.y() * 0.45, 1);

    // double const w = c.dims.x() * 0.75;
    // double const h = c.dims.y() * 0.75;
    // auto obj = Mesh<PosNorm> {
    //     { vec3i { 0, 1, 2 } },
    //     { { vec3 { -w/2, -h/2, 0 },
    //         vec3 {    0,    0, 1 } },
    //       { vec3 {    0,  h/2, 0 } ,
    //         vec3 {    0,    0, 1 } },
    //       { vec3 {  w/2, -h/2, 0 } ,
    //         vec3 {    0,    0, 1 } } },
    //     vec3 { 0, 0, 0 }
    // };
    // // auto obj = coloured(gobj, red);
    // // obj.rotate({ 0, 0, 1 }, 90);
    // obj.rotate({ 0, 1, 0 }, 180);

    double const cam_dist = radius * 2.;
    vec3 cam_base { 0, 0, cam_dist };
    // Cam cam { 90,
    //           cam_base,
    //           look_at(cam_base, {0, 0, 0})
    //           // look_at(cam_base, {0, 0, cam_dist + 1})
    // };
    Cam cam { look_at(cam_base, {0, 0, 0}),
              // look_at(cam_base, {0, 0, cam_dist + 1}),
              perspective<double>(deg2rad(120),
                                  // d.x()/d.y())
                                  // d.y()/d.x())
                                  1.)
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

        // // tc.write(1, 1, ascii2wstr(pr(cam.pos)));
        // for (size_t i = 0; i < cam.view.rows; i++)
        //     tc.write(1, 3 + i,
        //              ascii2wstr(pr(cam.view[i])));

        // double const amp = 25; (void)amp;
        // cam.pos.x() = cam_base.x() + cos(0.5 * t * M_PI) * amp;
        // cam.pos.y() = cam_base.y() + sin(0.5 * t * M_PI) * amp;
        // cam.pos.z() = cam_base.z() + cos(0.5 * t * M_PI) * amp;
        // cam.pos.z() = cam_base.z() - t * 5;

        // double const amp = 250; (void)amp;
        auto cam_pos = cam_base;
        vec3 tgt {0, 0, 0};
        // cam_pos.x() += cos(0.5 * t * M_PI) * amp;
        // cam_pos.y() += sin(0.5 * t * M_PI) * amp;

        auto const rot = rotor3f::angle_plane(
            -t, bivec3f { 0, 0, 1 });
        // auto const rot = rotor3f::angle_plane(
        //     deg2rad(95), bivec3f { 0, 0, 1 });
        // tgt = cam.pos + rotate(
        // tgt = cam_base + rotate(
        tgt = cam_pos + rotate(
            {0, 0, 1}, rot).to<vec3>();
        // tc.write(1, 8, ascii2wstr(pr(tgt - cam.pos)));
        // tc.write(1, 8, ascii2wstr(pr(tgt - cam_base)));
        // tc.write(1, 8, ascii2wstr(pr(tgt - cam_pos)));

        // tgt {sin(0.5 * t * M_PI) * amp, 0, 0};
        // tgt {cam.pos.x(), cam.pos.y(), 0};
        // tgt = cam.pos + vec3 {0,0,-1};

        cam.view =
            // look_at(cam.pos, tgt);
            // look_at(cam_base, tgt);
            look_at(cam_pos, tgt);

        obj.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        // obj.origin.y() = sin(0.5 * t * M_PI) * amp;
        flat.draw_line(obj, cam);

        // shell.rotate({ 0.5, 1, 0.75 }, 60 * dt);
        // // shell.origin.y() = sin(0.5 * t * M_PI) * amp;
        // unlit.draw(shell, cam, DrawMode::LINE);

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