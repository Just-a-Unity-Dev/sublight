#include "actor.hpp"
#include <libtcod.h>

namespace actordefs {
    const inline Actor player {
        0, 0, "@", tcod::ColorRGB{255,255,255}, true
    };
    const inline Actor human {
        0, 0, "@", TCOD_ColorRGB{255,255,0}, true
    };
}
