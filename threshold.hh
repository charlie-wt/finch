#pragma once

#include "geom.hh"

#include <vector>

struct ThresholdMap {
    ThresholdMap(uint64_t w, uint64_t h)
        : w(w), h(h), _vals(w * h, 0) { }

    ThresholdMap(uint64_t w, uint64_t h,
                 std::vector<uint64_t> vals)
        : w(w), h(h), _vals(vals) {
            if (_vals.size() != w * h)
                throw std::runtime_error(
                    "badly sized ThresholdMap "
                    "(wanted " + std::to_string(w) +
                    " × " + std::to_string(h) +
                    ") but got " +
                    std::to_string(_vals.size()) +
                    " values)");
        }

    uint64_t get (int64_t x, int64_t y) const {
        return _vals[(y % h) * h + (x % w)];
    }

    uint64_t &get (int64_t x, int64_t y) {
        return _vals[(y % h) * h + (x % w)];
    }

    uint64_t w, h;
    std::vector<uint64_t> _vals;
};

ThresholdMap bayer (uint64_t level);
