#include "termvis.hh"

#include <iostream>
#include <locale.h>

#include <ncursesw/ncurses.h>

using namespace std;


int main () {
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    cbreak();
    noecho();

    BrailleCanvas c(TermInfo::detect());
    /* TermInfo const ti { COLS, LINES }; */
    /* BrailleCanvas c(ti); */

    auto cb = cube(50) + vec3 { 30, 30, 0 };

    UpdateLoop(30, [&](double t,
                      double dt,
                      int64_t count) {
        c.clear();

        /* cb.rotate({ 0, 1, 0 }, 5 * dt); */
        cb.rot_x(10 * dt);
        cb.rot_z(10 * dt);

        /* cb.draw(c); */
        cb.project(c, 60, 80).draw(c);

        c.draw();

        return false;
    }).start();

    endwin();
}