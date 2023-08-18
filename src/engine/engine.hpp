#pragma once

#include <libtcod.hpp>
#include "actor.hpp"

class Engine {
    public:
        std::vector<Actor*> actors;
        Actor *player;
        // Map *map;

        Engine();
        ~Engine();
        void main_loop();
        void render();
        // void update();
        // void render();

        static tcod::Console g_console;
        static tcod::Context g_context;
};

extern Engine engine;
