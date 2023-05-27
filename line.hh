#pragma once

#include "point.hh"


struct Line {
    vec2 start, end;

    template<typename Canvas>
    void draw (Canvas &canvas) const {
        // bresenham's
        auto const st = Pixel::from_pt(start);
        auto const nd = Pixel::from_pt(end);

        auto const dx = abs(nd.x - st.x);
        auto const dy = -abs(nd.y - st.y);
        auto const xinc = st.x < nd.x ? 1 : -1;
        auto const yinc = st.y < nd.y ? 1 : -1;

        auto error = dx + dy;

        auto x = st.x;
        auto y = st.y;
        while (true) {
            canvas.set({ x, y });

            if (x == nd.x && y == nd.y)
                break;

            auto const error2 = 2 * error;

            if (error2 >= dy) {
                if (x == nd.x)
                    break;
                error += dy;
                x += xinc;
            }

            if (error2 <= dx) {
                if (y == nd.y)
                    break;
                error += dx;
                y += yinc;
            }
        }
    }
};

std::ostream &operator<< (std::ostream &os,
                          Line const &l);

struct Line3d {
    template<typename Canvas>
    void draw (Canvas &canvas) const {
        vec2 st { start[0], start[1] };
        vec2 nd { end[0], end[1] };
        Line { st, nd }.draw(canvas);
    }
    /* void draw (Canvas &canvas) const { */
    /*     Line { projected(start), */
    /*            projected(end) }.draw(canvas); */
    /* } */

    Line3d& operator+= (vec3 offset);
    Line3d& operator-= (vec3 offset);
    Line3d& operator*= (double scale);
    Line3d& operator/= (double scale);
    Line3d& operator%= (mat3 const &trans);

    vec3 start, end;
};

Line3d operator+ (Line3d l, vec3 offset);
Line3d operator- (Line3d l, vec3 offset);
Line3d operator* (Line3d l, double scale);
Line3d operator/ (Line3d l, double scale);
Line3d operator% (mat3 const &trans, Line3d l);

std::ostream &operator<< (std::ostream &os,
                          Line3d const &l);
