#pragma once

#include "colour.hh"
#include "geom.hh"
#include "threshold.hh"
#include "term.hh"

#include <vector>


// flattened, colour-per-pixel buffer of a frame,
// acting as if we were just a normal display;
// use so that we can worry about how to map this
// to canvases (eg. dithering, colour-resolution)
// separately from how to draw the scene.
struct ColourBuffer {
    ColourBuffer (pixel const &canvas_dims);

    void set (int64_t x, int64_t y,
              rgb col = white);
    void set (pixel p,
              rgb col = white)
        { set(p.x(), p.y(), col); }
    void clear ();

    rgb const &at (int64_t x, int64_t y) const;
    rgb &at (int64_t x, int64_t y);

    inline rgb const &at (pixel p) const
        { return at(p.x(), p.y()); }
    inline rgb &at (pixel p)
        { return at(p.x(), p.y()); }

    pixel dims;
    rgb bg;
    std::vector<rgb> data;
};

template <typename Canvas>
bool render_threshold(ColourBuffer const &buf,
                      Canvas &canvas) {
    for (int y = 0; y < buf.dims.y(); y++) {
        for (int x = 0; x < buf.dims.x(); x++) {
            /* TODO #enhancement: assuming
             * monochrome */
            bool const on = buf.at(x, y).r() > 127;
            canvas.set(x, y, on);
        }
    }
    return true;
}

template <typename Canvas>
bool render_rand(ColourBuffer const &buf,
                 Canvas &canvas) {
    for (int y = 0; y < buf.dims.y(); y++) {
        for (int x = 0; x < buf.dims.x(); x++) {
            /* TODO #enhancement: assuming
             * monochrome */
            double const adj = static_cast<double>(rand() % 127);
            bool const on = buf.at(x, y).r() + adj > 127;
            canvas.set(x, y, on);
        }
    }
    return true;
}

template <typename Canvas>
bool render_threshold_map(ColourBuffer const &buf,
                          Canvas &canvas,
                          ThresholdMap const &map) {
    for (int y = 0; y < buf.dims.y(); y++) {
        for (int x = 0; x < buf.dims.x(); x++) {
            /* TODO #enhancement: assuming
             * monochrome */
            bool const on = buf.at(x, y).r() >
                            map.get(x, y);
                            // 255 - map.get(x, y);
            canvas.set(x, y, on);
        }
    }
    return true;
}

template <typename Canvas>
bool render_col(ColourBuffer const &buf,
                Canvas &canvas) {
    for (int y = 0; y < buf.dims.y(); y++) {
        for (int x = 0; x < buf.dims.x(); x++) {
            /* TODO #finish */
            auto const px = buf.at(x, y);
            double const adj = static_cast<double>(rand() % 10) / 20.0;
            double const lightness = px.sum() / (3 * 255);
            bool const on = lightness + adj > 0.5;

            canvas.set(x, y, on);
        }
    }
    return true;
}
