#include "term.hh"

#include <sys/ioctl.h>
#include <stdio.h>


TermInfo TermInfo::detect () {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return { w.ws_col, w.ws_row };
}
