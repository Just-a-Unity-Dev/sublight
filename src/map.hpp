#pragma once
#include <libtcod.hpp>
#include "map.hpp"

struct Tile {
    bool canWalk;
    Tile() : canWalk(true) {}
};

class Map {
    public:
        int width, height;

        Map(int width, int height);
        ~Map();
        bool isWall(int x, int y) const;
        void render(tcod::Console& g_console) const;
    protected:
        std::vector<Tile> tiles;

        void setWall(int x, int y);
};
