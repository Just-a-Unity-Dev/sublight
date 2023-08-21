#include "actor.hpp"
#include <libtcod.h>

namespace actordefs {
    /// @brief Generic error actor. Just check whether the X is -1.
    inline Actor error {
        "error", -1, -1, "?", tcod::ColorRGB{255,0,0}, false
    };
    inline Actor player {
        "player", 0, 0, "@", tcod::ColorRGB{255,255,255}, true
    };
    inline Actor human {
        "human", 0, 0, "@", TCOD_ColorRGB{255,255,0}, true
    };
}
