#pragma once

#include "point.hh"


struct Line {
    Point start, end;

    template<typename Canvas>
    void draw (Canvas &canvas) const {
        // bresenham's
        auto const st = start.to_pixel();
        auto const nd = end.to_pixel();

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

struct Line3d {
    template<typename Canvas>
    void draw (Canvas &canvas) const {
        Line { start.perspective(),
               end.perspective() }.draw(canvas);
    }

    Line3d& operator+= (Point3d offset);
    Line3d& operator-= (Point3d offset);
    Line3d& operator*= (double scale);
    Line3d& operator/= (double scale);

    Point3d start, end;
};

Line3d operator+ (Line3d l, Point3d offset);
Line3d operator- (Line3d l, Point3d offset);
Line3d operator* (Line3d l, double scale);
Line3d operator/ (Line3d l, double scale);
