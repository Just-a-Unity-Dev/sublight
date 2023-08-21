#include <libtcod.hpp>
#include "actor.hpp"

Actor::Actor(char* name, int x, int y, std::string_view character, const TCOD_color_t &color, bool blocks_movement) :
    name(name), x(x),y(y),character(character),color(color), blocks_movement(blocks_movement) {};

// Moves the actor with a variable difference.
void Actor::move(int dx, int dy) {
    x += dx;
    y += dy;
}

// Updates the actor.
void Actor::update(tcod::Console &g_console, tcod::Context &g_context) {}
