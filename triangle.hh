#pragma once

#include "draw.hh"
#include "geom.hh"
#include "line.hh"


struct Tri {
    template <typename Canvas>
    void draw (Canvas &canvas, Cam const &cam,
               DrawMode mode) const {
        switch (mode) {
        case DrawMode::FILL:
            draw_fill(canvas, cam); break;
        case DrawMode::LINE:
            draw_lines(canvas, cam); break;
        }
    }

    template <typename Canvas>
    void draw_fill (Canvas &canvas,
                    Cam const &cam) const {
        // project
        std::array<pixel, 3> as_pixels;
        std::transform(verts.begin(), verts.end(),
                       as_pixels.begin(),
            [&](vec3 const &v) {
                vec3 const p = projected(v, canvas, cam);
                return p.to<pixel>();
            });

        // separate into above/below horizontal,
        // fill rows, lerp sides {in,out}wards
        std::sort(as_pixels.begin(),
                  as_pixels.end(),
            [](pixel const &lhs, pixel const &rhs) {
                return lhs.y()  < rhs.y();
            });

        pixel const low = as_pixels[0];
        pixel const mid = as_pixels[1];
        pixel const high = as_pixels[2];

        double const dy = high.y() - low.y();
        int64_t const other_mid_x =
            (high.x() - low.x()) *
                ((mid.y() - low.y()) / dy)
            + low.x();
        auto const left = std::min(mid.x(),
                                   other_mid_x);
        auto const right = std::max(mid.x(),
                                    other_mid_x);

        // low-mid
        if (low.y() != mid.y()) {
            auto const step = 1. / (mid.y() - low.y());
            auto const dl = left - low.x();
            auto const dr = right - low.x();
            for (int64_t y = low.y(); y < mid.y(); y++) {
                auto const prog = (y - low.y()) * step;
                auto const lft = low.x() + dl * prog;
                auto const rgt = low.x() + dr * prog;
                for (int64_t x = lft; x < rgt; x++)
                    canvas.set({ x, y });
            }
        }

        // mid-high
        if (mid.y() != high.y()) {
            auto const step = 1. / (high.y() - mid.y());
            auto const dl = high.x() - left;
            auto const dr = high.x() - right;
            for (int64_t y = mid.y(); y < high.y(); y++) {
                auto const prog = (y - mid.y()) * step;
                auto const lft = left + dl * prog;
                auto const rgt = right + dr * prog;
                for (int64_t x = lft; x < rgt; x++)
                    canvas.set({ x, y });
            }
        }
    }

    template <typename Canvas>
    void draw_line (Canvas &canvas,
                    Cam const &cam) const {
        ::draw_line(verts, canvas, cam);
    }

    Tri& operator+= (vec3 offset);
    Tri& operator-= (vec3 offset);
    Tri& operator*= (double scale);
    Tri& operator/= (double scale);
    Tri& operator%= (mat3 const &trans);

    std::array<vec3, 3> verts;
};

Tri operator+ (Tri l, vec3 offset);
Tri operator- (Tri l, vec3 offset);
Tri operator* (Tri l, double scale);
Tri operator/ (Tri l, double scale);
Tri operator% (mat3 const &trans, Tri l);
