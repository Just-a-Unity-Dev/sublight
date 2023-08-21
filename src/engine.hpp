#pragma once

#include <libtcod.hpp>
#include "actor.hpp"
#include "map.hpp"

struct InputHandler {
    void move_action(int dx, int dy);
    void attack_action(int dx, int dy);
    void bump_action(int dx, int dy);
    void wait_action();
};

/// Main engine struct that holds actors, and handles rendering/updating.
struct Engine {
    Map map;

    Engine();
    ~Engine();
    void main_loop();
    void render();
    void update();

    void handle_npc_turns();

    int fov_radius;
    bool compute_fov;

    InputHandler handler;
    Actor& get_player();

    tcod::Console g_console;
    tcod::Context g_context;
};

extern Engine engine;
