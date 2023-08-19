#pragma once
#include <libtcod.h>

struct Tile {
    bool can_walk;
    std::string_view character;
    TCOD_color_t foreground;
    TCOD_color_t background;
    TCOD_color_t dark;
    bool transparent;
};

namespace tiledefs {
    inline const Tile floor {
        true, ".", tcod::ColorRGB{35,35,35}, tcod::ColorRGB{25,25,25}, tcod::ColorRGB{15,15,15}, true
    };
};
