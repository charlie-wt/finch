#pragma once

#include "term.hh"

#include <ncursesw/ncurses.h>

#include <string>
#include <vector>


struct Text {
    int64_t x, y;
    std::wstring data;
};


struct TextCanvas {
    TextCanvas(pixel dims)
        : dims(dims), text() {}

    void write (Text const &t) {
        /* TODO #bug: clip string to within canvas
         *            bounds */
        text.push_back(t);
    }

    Text& write (int64_t x, int64_t y,
                std::wstring const &str) {
        Text const t { x, y, str };
        write(t);
        return text.back();
    }

    void clear () {
        text.clear();
    }

    void draw () const {
        for (auto const &str : text)
            mvaddwstr(str.y, str.x, str.data.data());
        ::refresh();
    }

    pixel dims;
    std::vector<Text> text;
};
