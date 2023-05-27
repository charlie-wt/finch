#include "term.hh"

#include <sys/ioctl.h>
#include <stdio.h>

#include <ncursesw/ncurses.h>


TermInfo TermInfo::detect () {
    return { COLS, LINES };
    /* struct winsize w; */
    /* ioctl(0, TIOCGWINSZ, &w); */
    /* return { w.ws_col, w.ws_row }; */
}
