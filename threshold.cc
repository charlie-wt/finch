#include "threshold.hh"

#include <cmath>


ThresholdMap bayer (uint64_t level) {
    ThresholdMap const base { 2, 2, { 0, 2,
                                      3, 1 } };
    auto prev = base;
    uint64_t current_level = 0;

    while (current_level < level) {
        ThresholdMap curr(prev.w * 2, prev.h * 2);
        float const x_sc = static_cast<float>(base.w)/curr.w;
        float const y_sc = static_cast<float>(base.h)/curr.h;

        for (uint64_t y = 0; y < curr.h; y++) {
            for (uint64_t x = 0; x < curr.w; x++) {
                curr.get(x, y) =
                    4 * prev.get(x, y) +
                    base.get(x * x_sc, y * y_sc);
            }
        }

        prev = curr;
        current_level++;
    }

    float scale = 255. / std::powf(2, 2 * level + 2);
    std::transform(prev._vals.cbegin(),
                   prev._vals.cend(),
                   prev._vals.begin(),
                   [scale](uint64_t v) { return v * scale; });
    return prev;
}
