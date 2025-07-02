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

    // TODO #enhancement: split by lines,
    //                    potentially create many
    //                    `Text`s (drawing doesn't
    //                    like newlines).
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

// very basic; no multibyte char handling
inline std::wstring ascii2wstr (std::string const &str) {
    std::wstring res {};
    res.reserve(str.size());
    for (auto const c : str)
        res += static_cast<wchar_t>(c);
    return res;
}
