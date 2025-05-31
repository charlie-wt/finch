#pragma once

#include "geom.hh"

#include <string>


enum class DrawMode {
    FILL,
    LINE
};

/* TODO #cleanup: framebuffers, not canvases */

template <typename Canvas>
void draw_fill (std::array<vec3, 3> verts,
                Canvas &canvas,
                Cam const &cam) {
    // project
    std::array<vec3, 3> proj;
    std::transform(verts.begin(), verts.end(),
                   proj.begin(),
        [&](vec3 const &v) {
            return cam.projected(v, canvas);
        });

    // separate into above/below horizontal,
    // fill rows, lerp sides {in,out}wards
    std::sort(proj.begin(),
              proj.end(),
        [](vec3 const &lhs, vec3 const &rhs) {
            return lhs.y() < rhs.y();
        });

    pixel const low = proj[0].to<pixel>();
    pixel const mid = proj[1].to<pixel>();
    pixel const high = proj[2].to<pixel>();

    double const dy = high.y() - low.y();
    int64_t const other_mid_x =
        (high.x() - low.x()) *
            ((mid.y() - low.y()) / dy)
        + low.x();
    auto const left = std::min(mid.x(), other_mid_x);
    auto const right = std::max(mid.x(), other_mid_x);

    vec3 const flow = proj[0];
    vec3 const fmid = proj[1];
    vec3 const fhigh = proj[2];

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
                canvas.set(x, y, z);
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
                canvas.set(x, y, z);
            }
        }
    }
}

template<typename Canvas>
void draw (vec2 start, vec2 end,
           Canvas &canvas) {
    // bresenham's
    auto const st = start.to<pixel>();
    auto const nd = end.to<pixel>();

    auto const dx = abs(nd.x() - st.x());
    auto const dy = -abs(nd.y() - st.y());
    auto const xinc = st.x() < nd.x() ? 1 : -1;
    auto const yinc = st.y() < nd.y() ? 1 : -1;

    auto error = dx + dy;

    auto x = st.x();
    auto y = st.y();
    while (true) {
        canvas.set(x, y);

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

template<typename Canvas>
void draw_line (vec3 start, vec3 end,
                Canvas &canvas, Cam const &cam) {
    vec3 const sproj = cam.projected(start, canvas);
    vec3 const eproj = cam.projected(end, canvas);

    // bresenham's
    auto const st = sproj.to<pixel>();
    auto const nd = eproj.to<pixel>();

    auto const dx = abs(nd.x() - st.x());
    auto const dy = -abs(nd.y() - st.y());
    auto const xinc = st.x() < nd.x() ? 1 : -1;
    auto const yinc = st.y() < nd.y() ? 1 : -1;

    auto error = dx + dy;

    double const dz = eproj.z() - sproj.z();
    double const zstp = dx == 0
        ? 0
        : dz / (nd.x() - st.x());

    auto x = st.x();
    auto y = st.y();
    while (true) {
        /* TODO #correctness: x is discretised */
        double const z = start.z() +
            (x - st.x()) * zstp;
        canvas.set(x, y, z);

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

template <typename Canvas>
void draw_line (std::array<vec3, 3> verts,
                Canvas &canvas,
                Cam const &cam) {
    draw_line(verts[0], verts[1], canvas, cam);
    draw_line(verts[1], verts[2], canvas, cam);
    draw_line(verts[2], verts[0], canvas, cam);
}
