#pragma once

#include <libtcod.hpp>
#include "actor.hpp"
#include "map.hpp"

struct InputHandler {
    void move_action(int dx, int dy);
    void attack_action(int dx, int dy);
    void toggle_combat_action();
    void bump_action(int dx, int dy);
};

/// Main engine struct that holds actors, and handles rendering/updating.
struct Engine {
    Map map;

    Engine();
    ~Engine();
    void main_loop();
    void render();
    void update();

    int fov_radius;
    bool compute_fov;
    bool combat_mode;

    InputHandler handler;
    Actor& get_player();

    tcod::Console g_console;
    tcod::Context g_context;
};

extern Engine engine;
