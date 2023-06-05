#pragma once

#include "triangle.hh"


struct Mesh {

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
        /* TODO #finish */
        for (auto const &t : tris)
            t.draw_fill(canvas, cam);
    }

    template <typename Canvas>
    void draw_lines (Canvas &canvas,
                     Cam const &cam) const {
        for (auto const &t : tris)
            t.draw_lines(canvas, cam);
    }

    std::vector<Tri> tris;
};
