#pragma once

class AI {
    void update();
};

class Actor {
    public:
        const char* name;
        int x;
        int y;
        std::string_view character;
        TCOD_color_t color;
        bool blocks_movement;

        Actor(char* name, int x, int y, std::string_view character, const TCOD_color_t &color, bool blocks_movement);
        void update(tcod::Console &g_console, tcod::Context &g_context);
        void move(int dx, int dy);
};
