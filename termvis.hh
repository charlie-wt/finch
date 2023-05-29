#pragma once

#include "braille.hh"
#include "char.hh"
#include "geom.hh"
#include "line.hh"
#include "shape.hh"
#include "term.hh"
#include "text.hh"
#include "time.hh"
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
