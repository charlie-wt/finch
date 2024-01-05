#pragma once

#include "char.hh"
#include "depth.hh"
#include "framebuffer.hh"
#include "term.hh"
#include "xmesh.hh"


// lit but uncoloured
struct LitShader {
    LitShader (TermInfo const &t);

    void draw (XMesh<PosNorm> const &mesh,
               Cam const &cam);

    void draw (std::array<PosNorm, 3> verts);

    void clear ();

    template <typename Canvas>
    bool render_to (Canvas &canvas) {
        if (canvas.w != buf.w ||
            canvas.h != buf.h) {
            return false;
        }

        /* TODO #finish: dither */
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

    template <>
    bool render_to (CharCanvas &canvas) {
        if (canvas.w != buf.w ||
            canvas.h != buf.h)
            return false;

        /* TODO #finish: dither */
        for (int y = 0; y < buf.h; y++) {
            for (int x = 0; x < buf.w; x++) {
                /* TODO #enhancement: assuming
                 * monochrome */
                char const ch = buf.at(x, y).r() > 0.5
                    ? '*'
                    : ' ';
                canvas.set(x, y, ch);
            }
        }

        return true;
    }

    Framebuffer buf;
    DepthBuffer depth_buf;
    bool lol;
};
