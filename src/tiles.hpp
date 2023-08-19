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
    struct Floor {
        bool can_walk = true;
        std::string_view character = ".";
        TCOD_color_t foreground = tcod::ColorRGB{255,255,255};
        TCOD_color_t background = tcod::ColorRGB{255,255,255};
        TCOD_color_t dark = tcod::ColorRGB{125,125,125};
        bool transparent = true;
    };
}
