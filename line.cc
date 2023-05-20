#include "line.hh"


void Line::draw (CharGrid &cg) const {
    // bresenham's
    auto const st = start.to_pixel();
    auto const nd = end.to_pixel();

    auto const left = std::min(st.x, nd.x);
    auto const right = std::max(st.x, nd.x);
    auto const top = std::min(st.y, nd.y);
    auto const bottom = std::max(st.y, nd.y);

    auto const dx = right - left;
    auto const dy = bottom - top;

    if (dx > dy) {
        auto D = 2 * dy - dx;

        auto y = top;
        for (int64_t x = left; x < right; x++) {
            cg.set({ x, y });

            if (D > 0) {
                y++;
                D -= 2 * dx;
            }

            D += 2 * dy;
        }
    } else {
        auto D = 2 * dx - dy;

        auto x = left;
        for (int64_t y = top; y < bottom; y++) {
            cg.set({ x, y });

            if (D > 0) {
                x++;
                D -= 2 * dy;
            }

            D += 2 * dx;
        }
    }
}
