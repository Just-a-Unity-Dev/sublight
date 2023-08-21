#pragma once

#include <libtcod.hpp>
#include "actor.hpp"
#include "map.hpp"

/// Main engine struct that holds actors, and handles rendering/updating.
struct Engine {
    Map map;

    Engine();
    ~Engine();
    void main_loop();
    void render();
    void update();
    Actor& get_player();
    int fov_radius;
    bool compute_fov;

    tcod::Console g_console;
    tcod::Context g_context;
};

extern Engine engine;
