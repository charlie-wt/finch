#pragma once

#include "char.hh"
#include "depth.hh"
#include "draw.hh"
#include "framebuffer.hh"
#include "term.hh"
#include "xmesh.hh"


template<typename VertSh,
         typename FragSh,
         typename RenderFn>
struct Shader {
    Shader (TermInfo const &t,
            VertSh const &vert_shader,
            FragSh const &frag_shader,
            RenderFn const &render_fn)
        : buf(t)
        , depth_buf(t)
        , vert_shader(vert_shader)
        , frag_shader(frag_shader)
        , render_fn(render_fn) {}

    template <typename Vert>
    void draw (XMesh<Vert> const &mesh,
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
    void draw_fill (XMesh<Vert> const &mesh,
                    Cam const &cam) {
        for (auto const &idx : mesh.indices) {
            std::array<Vert, 3> verts =
                { mesh.verts[idx[0]],
                  mesh.verts[idx[1]],
                  mesh.verts[idx[2]] };

            for (auto &v : verts)
                v = vert_shader(v, mesh, cam, buf);

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
                    if (depth_buf.set(x, y, z)) {
                        /* TODO #cleanup */
                        vec3 fpx
                            { static_cast<double>(x),
                              static_cast<double>(y),
                              static_cast<double>(z) };
                        vec3 const bc = barycentric(
                            fpx, flow, fmid, fhigh);
                        buf.set(x, y,
                                frag_shader(
                                    fpx, bc, verts));
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
                    if (depth_buf.set(x, y, z)) {
                        /* TODO #cleanup */
                        vec3 fpx
                            { static_cast<double>(x),
                              static_cast<double>(y),
                              static_cast<double>(z) };
                        vec3 const bc = barycentric(
                            fpx, flow, fmid, fhigh);
                        buf.set(x, y,
                                frag_shader(
                                    fpx, bc, verts));
                    }
                }
            }
        }
    }

    template <typename Vert>
    void draw_line (XMesh<Vert> const &mesh,
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
            while (true) {
                /* TODO #correctness: x is discretised */
                double const z = start.z() +
                    (x - st.x()) * zstp;
                if (depth_buf.set(x, y, z)) {
                    /* TODO #cleanup */
                    vec3 fpx
                        { static_cast<double>(x),
                          static_cast<double>(y),
                          static_cast<double>(z) };
                    vec3 const bc = barycentric(
                        fpx, verts[0].pos,
                        verts[1].pos, verts[2].pos);
                    buf.set(x, y,
                            frag_shader(
                                fpx, bc, verts));
                }

                if (x == nd.x() && y == nd.y())
                    break;

                auto const error2 = 2 * error;

                if (error2 >= dy) {
                    if (x == nd.x())

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
        depth_buf.clear();
    }

    template <typename Canvas>
    bool render_to (Canvas &canvas) {
        if (canvas.w != buf.w ||
            canvas.h != buf.h) {
            return false;
        }

        return render_fn(buf, canvas);
    }

    Framebuffer buf;
    DepthBuffer depth_buf;

    VertSh vert_shader;
    FragSh frag_shader;
    RenderFn render_fn;
};

template <typename Vert>
auto unlit_shader (TermInfo const &ti) {
    return Shader(
        TermInfo { ti.w * 2, ti.h * 4 },
        [](Vert v,
           XMesh<Vert> const &mesh,
           Cam const &cam,
           Framebuffer const &buf) {
            // apply origin & project
            v.pos = projected(
                v.pos + mesh.origin,
                buf, cam);
            return v;
        },
        [](vec3 pos,
           vec3 bc,
           std::array<Vert, 3> verts) {
            (void)pos; (void)bc; (void)verts;
            return rgb::ones();
        },
        [](Framebuffer const &buf,
           auto &canvas) {
            for (int y = 0; y < buf.h; y++) {
                for (int x = 0; x < buf.w; x++) {
                    /* TODO #enhancement: assuming
                     * monochrome */
                    bool const on = buf.at(x, y).r() > 0.5;
                    canvas.set(x, y, 0, on);
                }
            }
            return true;
        }
    );
}
template <typename Vert>
auto lit_shader (TermInfo const &ti) {
    return Shader(
        TermInfo { ti.w * 2, ti.h * 4 },
        [](Vert v,
           XMesh<Vert> const &mesh,
           Cam const &cam,
           Framebuffer const &buf) {
            // apply origin & project
            v.pos = projected(
                v.pos + mesh.origin,
                buf, cam);
            return v;
        },
        [](vec3 pos,
           vec3 bc,
           std::array<Vert, 3> verts) {
            (void)pos;
            vec3 const nm {
                verts[0].norm * bc[0] +
                verts[1].norm * bc[1] +
                verts[2].norm * bc[2]
            };
            double lgt = fabs(nm.dot(vec3 {0,0,-1}));
            return rgb { lgt, lgt, lgt };
        },
        [](Framebuffer const &buf,
           auto &canvas) {
            for (int y = 0; y < buf.h; y++) {
                for (int x = 0; x < buf.w; x++) {
                    /* TODO #enhancement: assuming
                     * monochrome */
                    double const adj = static_cast<double>(rand() % 10) / 20.0;
                    bool const on = buf.at(x, y).r() + adj > 0.5;
                    canvas.set(x, y, 0, on);
                }
            }
            return true;
        }
    );
}

template <typename Vert>
auto flat_lit_shader (TermInfo const &ti) {
    return Shader(
        TermInfo { ti.w * 2, ti.h * 4 },
        [](Vert v,
           XMesh<Vert> const &mesh,
           Cam const &cam,
           Framebuffer const &buf) {
            // apply origin & project
            v.pos = projected(
                v.pos + mesh.origin,
                buf, cam);
            return v;
        },
        [](vec3 pos,
           vec3 bc,
           std::array<Vert, 3> verts) {
            (void)pos; (void)bc;
            vec3 const nm = norm(cross(
                verts[1].pos - verts[0].pos,
                verts[2].pos - verts[0].pos));
            double lgt = fabs(nm.dot(vec3 {0,0,-1}));
            return rgb { lgt, lgt, lgt };
        },
        [](Framebuffer const &buf,
           auto &canvas) {
            for (int y = 0; y < buf.h; y++) {
                for (int x = 0; x < buf.w; x++) {
                    /* TODO #enhancement: assuming
                     * monochrome */
                    double const adj = static_cast<double>(rand() % 10) / 20.0;
                    bool const on = buf.at(x, y).r() + adj > 0.5;
                    canvas.set(x, y, 0, on);
                }
            }
            return true;
        }
    );
}
