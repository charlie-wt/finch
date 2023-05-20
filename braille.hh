#pragma once

#include "term.hh"

#include <string>


struct Block {
    // essentially a bitfield, top-left to
    // bottom-right, vertical-first.
    uint8_t data;

    wchar_t to_char () const;
};

struct BrailleGrid {
    BrailleGrid (TermInfo const &t);

    int64_t w, h;
    std::string data;
};
