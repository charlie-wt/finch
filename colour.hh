#pragma once

#include "geom.hh"


using rgb = vec<uint8_t, 3>;
using rgba = vec<uint8_t, 4>;

rgb constexpr black     {0, 0, 0};
rgb constexpr white     {255, 255, 255};
rgb constexpr grey      {127, 127, 127};
rgb constexpr red       {255, 0, 0};
rgb constexpr green     {0, 255, 0};
rgb constexpr blue      {0, 0, 255};
rgb constexpr yellow    {255, 255, 0};
rgb constexpr cyan      {0, 255, 255};
rgb constexpr magenta   {255, 0, 255};
rgb constexpr orange    {255, 127, 0};
rgb constexpr purple    {255, 0, 127};
rgb constexpr turquoise {0, 255, 127};
