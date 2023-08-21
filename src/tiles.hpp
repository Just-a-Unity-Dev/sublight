#pragma once
#include <libtcod.h>

struct Tile {
    bool can_walk = false;
    std::string_view character = " ";
    TCOD_color_t foreground = tcod::ColorRGB{15,15,15};
    TCOD_color_t background = tcod::ColorRGB{35,35,35};
    bool transparent = false;
};

namespace tiledefs {
    inline const Tile floor {
        true, ".", tcod::ColorRGB{35,35,35}, tcod::ColorRGB{25,25,25}, true
    };
    inline const Tile wall {
        true, ".", tcod::ColorRGB{0, 0, 0}, tcod::ColorRGB{35, 35, 35}, true
    };
};
