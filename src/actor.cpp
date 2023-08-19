#include <libtcod.hpp>
#include "actor.hpp"

Actor::Actor(int x, int y, std::string_view character, const TCOD_color_t &color) :
    x(x),y(y),character(character),color(color) {};

// Moves the actor with a variable difference, accounts for collisions.
void Actor::move(int dx, int dy) {
    x += dx;
    y += dy;
}

// Updates the actor.
void Actor::update(tcod::Console &g_console, tcod::Context &g_context) {}
