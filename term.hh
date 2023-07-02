#pragma once

#include <cstdint>


struct TermInfo {
    static TermInfo detect();

    int64_t w, h;
};
