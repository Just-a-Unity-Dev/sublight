#pragma once
#include <libtcod.h>

struct Tile {
    bool can_walk = false;
    std::string_view character = "▓";
    TCOD_color_t foreground = tcod::ColorRGB{175,175,175};
    TCOD_color_t background = tcod::ColorRGB{0,0,0};
    TCOD_color_t dark = tcod::ColorRGB{125,125,125};
    bool transparent = false;
};

namespace tiledefs {
    struct Floor : Tile {
        bool can_walk = true;
        std::string_view character = ".";
        TCOD_color_t foreground = tcod::ColorRGB{255,255,255};
        TCOD_color_t background = tcod::ColorRGB{255,255,255};
        TCOD_color_t dark = tcod::ColorRGB{125,125,125};
        bool transparent = false;
    };
}
