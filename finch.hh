#pragma once

#include "braille.hh"
#include "camera.hh"
#include "char.hh"
#include "draw.hh"
#include "geom.hh"
#include "line.hh"
#include "mesh.hh"
#include "shape.hh"
#include "term.hh"
#include "text.hh"
#include "time.hh"
#include "triangle.hh"
#include "util.hh"

#include <locale.h>

#include <ncursesw/ncurses.h>


inline void init () {
    // ncurses
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
}

inline void teardown () {
    // ncurses
    endwin();
}
