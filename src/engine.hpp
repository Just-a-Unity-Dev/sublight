#ifndef _TOOLS_H_
#define _TOOLS_H_

#pragma once

#include <libtcod.hpp>
#include "actor.hpp"
#include "map.hpp"

/// Main engine struct that holds actors, and handles rendering/updating.
struct Engine {
    std::vector<Actor> actors;
    Map map;

    Engine();
    ~Engine();
    void main_loop();
    void render();
    void update();
    Actor& get_player();

    tcod::Console g_console;
    tcod::Context g_context;
};

extern Engine engine;
#endif
