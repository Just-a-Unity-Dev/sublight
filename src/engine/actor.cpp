#include <libtcod.hpp>
#include "actor.hpp"

Actor::Actor(int x, int y, std::string_view character, const TCOD_color_t &color) :
    x(x),y(y),character(character),color(color) {};
