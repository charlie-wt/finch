#pragma once

#include "char.hh"
#include "colour.hh"
#include "depth.hh"
#include "draw.hh"
#include "colourbuffer.hh"
#include "mesh.hh"
#include "term.hh"


struct Framebuffer {
    Framebuffer (pixel canvas_dims)
        : col(canvas_dims)
        , depth(canvas_dims)
        , dims(canvas_dims) {}

    void clear () {
        col.clear();
        depth.clear();
    }

    ColourBuffer col;
    DepthBuffer depth;
    pixel dims;
};

template<typename VertSh, typename FragSh>
struct Shader {
    Shader (Framebuffer &buf,
            VertSh const &vert_shader,
            FragSh const &frag_shader)
        : buf(buf)
        , vert_shader(vert_shader)
        , frag_shader(frag_shader) {}

    template <typename Vert>
    void draw (Mesh<Vert> const &mesh,
               Cam const &cam,
               DrawMode mode = DrawMode::FILL) {
        switch (mode) {
        case DrawMode::FILL:
            draw_fill(mesh, cam);
            break;
        case DrawMode::LINE:
            draw_line(mesh, cam);
            break;
        }
    }

    template <typename Vert>
    void draw_fill (Mesh<Vert> const &mesh,
                    Cam const &cam) {
        for (auto const &idx : mesh.indices) {
            std::array<Vert, 3> verts =
                { mesh.verts[idx[0]],
                  mesh.verts[idx[1]],
                  mesh.verts[idx[2]] };

            // TODO #finish: some notion of
            // 'scratch space', for things like
            // uniforms & extra attributes. can't
            // just have a member of Shader, as
            // that eould be stored per-shader,
            // not per-draw call.
            for (auto &v : verts)
                v = vert_shader(v, mesh, cam, buf);

            // TODO #bug? or just make the mesh
            // right?
            // // back-face culling
            // constexpr bool has_norms =
            //     requires(Vert const &v) { v.norm; };
            // if constexpr (has_norms) {
            //     auto const nm = vec3 {
            //         verts[0].norm +
            //         verts[1].norm +
            //         verts[2].norm
            //     } / 3.0;
            //     if (nm.dot(vec3 {0,0,1}) > 0)
            //         continue;
            // }

            draw_fill(verts);
        }
    }

    template <typename Vert>
    void draw_fill (std::array<Vert, 3> verts) {
        // separate into above/below horizontal,
        // fill rows, lerp sides {in,out}wards
        std::sort(verts.begin(),
                  verts.end(),
            [](Vert const &lhs, Vert const &rhs) {
                return lhs.pos.y() < rhs.pos.y();
            });

        pixel const low = verts[0].pos.template to<pixel>();
        pixel const mid = verts[1].pos.template to<pixel>();
        pixel const high = verts[2].pos.template to<pixel>();

        double const dy = high.y() - low.y();
        int64_t const other_mid_x =
            (high.x() - low.x()) *
                ((mid.y() - low.y()) / dy)
            + low.x();
        auto const left = std::min(mid.x(), other_mid_x);
        auto const right = std::max(mid.x(), other_mid_x);

        vec3 const flow = verts[0].pos;
        vec3 const fmid = verts[1].pos;
        vec3 const fhigh = verts[2].pos;

        double const other_mid_z =
            (fhigh.z() - flow.z()) *
                ((fmid.y() - flow.y()) / dy)
            + flow.z();

        auto left_z = fmid.z();
        auto right_z = other_mid_z;
        if (left != mid.x()) {
            left_z = other_mid_z;
            right_z = fmid.z();
        }

        // low-mid
        if (low.y() != mid.y()) {
            auto const step = 1. / (mid.y() - low.y());
            auto const dl = left - low.x();
            auto const dr = right - low.x();

            auto const dlz = left_z - flow.z();
            auto const drz = right_z - flow.z();
            for (int64_t y = low.y(); y < mid.y(); y++) {
                auto const prog = (y - low.y()) * step;

                int64_t const lft = low.x() + dl * prog;
                int64_t const rgt = low.x() + dr * prog;

                auto const lftz = flow.z() + dlz * prog;
                auto const rgtz = flow.z() + drz * prog;
                auto const z_step = (rgtz - lftz) /
                    (rgt - lft);
                for (int64_t x = lft; x < rgt; x++) {
                    double const z =
                        lftz + (x - lft) * z_step;
                    // auto const sc = screen(
                    //         vec2i { x, y }, buf);
                    if (buf.depth.set(x, y, z)) {
                    // if (buf.depth.set(sc.x(), sc.y(), z)) {
                        /* TODO #cleanup */
                        vec3 fpx
                            { static_cast<double>(x),
                              static_cast<double>(y),
                              static_cast<double>(z) };
                        vec3 const bc = barycentric(
                            fpx, flow, fmid, fhigh);
                        buf.col.set(x, y,
                        // buf.col.set(sc.x(), sc.y(),
                            frag_shader(fpx, bc, verts));
                    }
                }
            }
        }

        // mid-high
        if (mid.y() != high.y()) {
            auto const step = 1. / (high.y() - mid.y());
            auto const dl = high.x() - left;
            auto const dr = high.x() - right;

            auto const dlz = fhigh.z() - left_z;
            auto const drz = fhigh.z() - right_z;
            for (int64_t y = mid.y(); y < high.y(); y++) {
                auto const prog = (y - mid.y()) * step;

                int64_t const lft = left + dl * prog;
                int64_t const rgt = right + dr * prog;

                auto const lftz = left_z + dlz * prog;
                auto const rgtz = right_z + drz * prog;
                auto const z_step = (rgtz - lftz) /
                    (rgt - lft);
                for (int64_t x = lft; x < rgt; x++) {
                    double const z =
                        lftz + (x - lft) * z_step;
                    if (buf.depth.set(x, y, z)) {
                        /* TODO #cleanup */
                        vec3 fpx
                            { static_cast<double>(x),
                              static_cast<double>(y),
                              static_cast<double>(z) };
                        vec3 const bc = barycentric(
                            fpx, flow, fmid, fhigh);
                        buf.col.set(x, y,
                            frag_shader(fpx, bc, verts));
                    }
                }
            }
        }
    }

    template <typename Vert>
    void draw_line (Mesh<Vert> const &mesh,
                    Cam const &cam) {
        for (auto const &idx : mesh.indices) {
            std::array<Vert, 3> verts =
                { mesh.verts[idx[0]],
                  mesh.verts[idx[1]],
                  mesh.verts[idx[2]] };

            for (auto &v : verts)
                v = vert_shader(v, mesh, cam, buf);

            draw_line(verts);
        }
    }

    template <typename Vert>
    void draw_line (std::array<Vert, 3> verts) {
        for (size_t i = 0; i < 3; i++) {
            // bresenham's
            auto const start = verts[i].pos;
            auto const end  = verts[(i+1) % 3].pos;

            auto const st = start.template to<pixel>();
            auto const nd = end.template to<pixel>();

            auto const dx = abs(nd.x() - st.x());
            auto const dy = -abs(nd.y() - st.y());
            auto const xinc = st.x() < nd.x() ? 1 : -1;
            auto const yinc = st.y() < nd.y() ? 1 : -1;

            auto error = dx + dy;

            double const dz = end.z() - start.z();
            double const zstp = dx == 0
                ? 0
                : dz / (nd.x() - st.x());

            auto x = st.x();
            auto y = st.y();
            bool was_oob = false;
            while (true) {
                bool const oob =
                    x < 0 || x > buf.dims.x() ||
                    y < 0 || y > buf.dims.y();
                if (!was_oob && oob)
                    break;
                was_oob = oob;

                /* TODO #correctness: x is discretised */
                double const z = start.z() +
                    (x - st.x()) * zstp;
                if (buf.depth.set(x, y, z)) {
                    /* TODO #cleanup */
                    vec3 fpx
                        { static_cast<double>(x),
                          static_cast<double>(y),
                          static_cast<double>(z) };
                    vec3 const bc = barycentric(
                        fpx, verts[0].pos,
                        verts[1].pos, verts[2].pos);
                    buf.col.set(x, y,
                        frag_shader(fpx, bc, verts));
                }

                if (x == nd.x() && y == nd.y())
                    break;

                auto const error2 = 2 * error;

                if (error2 >= dy) {
                    if (x == nd.x())
                        break;
                    error += dy;
                    x += xinc;
                }

                if (error2 <= dx) {
                    if (y == nd.y())
                        break;
                    error += dx;
                    y += yinc;
                }
            }
        }
    }

    void clear () {
        buf.clear();
    }

    /* template <typename Canvas> */
    /* bool render_to (Canvas &canvas) { */
    /*     if (canvas.dims != buf.dims) */
    /*         return false; */
    /*     return render_fn(buf, canvas); */
    /* } */

    Framebuffer &buf;

    VertSh vert_shader;
    FragSh frag_shader;
};

template <typename Vert>
auto simple_vert () {
    // TODO #remove
    // return [](Vert v,
    //           Mesh<Vert> const &mesh,
    //           Cam const &cam,
    //           Framebuffer const &buf) {
    //     // apply origin & project
    //     v.pos = cam.projected(
    //         v.pos + mesh.origin,
    //         buf.col);
    //     return v;
    // };
    return [](Vert v,
              Mesh<Vert> const &mesh,
              Cam const &cam,
              Framebuffer const &buf) {
        // apply origin & project
        // TODO #remove
        // auto const moved =
        //     v.pos + mesh.origin;
        // vec4 const as4
        //     {moved.x(), moved.y(), moved.z(), 1.0};
        // auto const looked =
        //     // (cam.view % as4).to<vec3>();
        //     // TODO #speed: cache view_proj
        //     (cam.view_proj() % as4).to<vec3>();
        // auto const looked = cam.view @ as4;
        // v.pos = screen(
        v.pos = cam.projected(
            v.pos + mesh.origin,
            // looked,
            buf.col);

        return v;
    };
}

template <typename Vert>
auto unlit_shader (Framebuffer &buf) {
    return Shader(
        buf,
        simple_vert<Vert>(),
        [](vec3 pos,
           vec3 bc,
           std::array<Vert, 3> verts) {
            (void)pos; (void)bc; (void)verts;
            return white;
        }
    );
}

template <typename Vert>
auto lit_shader (Framebuffer &buf) {
    return Shader(
        buf,
        simple_vert<Vert>(),
        [](vec3 pos,
           vec3 bc,
           std::array<Vert, 3> verts) {
            (void)pos;
            vec3 const nm {
                verts[0].norm * bc[0] +
                verts[1].norm * bc[1] +
                verts[2].norm * bc[2]
            };
            uint8_t const lgt = fabs(nm.dot(vec3 {0,0,1})) * 255;
            return rgb { lgt, lgt, lgt };
        }
    );
}

template <typename Vert>
auto flat_lit_shader (Framebuffer &buf) {
    return Shader(
        buf,
        simple_vert<Vert>(),
        [](vec3 pos,
           vec3 bc,
           std::array<Vert, 3> verts) {
            (void)pos; (void)bc;
            vec3 const nm = norm(cross(
                verts[1].pos - verts[0].pos,
                verts[2].pos - verts[0].pos));
            // TODO #bug: need to record
            // world-space coords & dot with cam,
            // or record view-space & dot w/ fwd
            uint8_t const lgt = fabs(nm.dot(vec3 {0,0,1})) * 255;
            return rgb { lgt, lgt, lgt };
        }
    );
}

/* TODO #cleanup: make this the non col_ one */
template <typename Vert>
auto flat_lit_col_shader (Framebuffer &buf) {
    return Shader(
        buf,
        simple_vert<Vert>(),
        [](vec3 pos,
           vec3 bc,
           std::array<Vert, 3> verts) {
            (void)pos; (void)bc;
            vec3 const nm = norm(cross(
                verts[1].pos - verts[0].pos,
                verts[2].pos - verts[0].pos));
            uint8_t const lgt = fabs(nm.dot(vec3 {0,0,1})) * 255;
            rgb const col {
                verts[0].col * bc[0] +
                verts[1].col * bc[1] +
                verts[2].col * bc[2]
            };
            return lgt * col;
        }
    );
}
