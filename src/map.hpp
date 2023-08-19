#pragma once
#include <libtcod.hpp>
#include "actor.hpp"

struct Tile {
    bool canWalk = true;
};

struct RectangularRoom {
    int x1;
    int x2;
    int y1;
    int y2;

    RectangularRoom(int x, int y, int width, int height) : x1(x), x2(x + width), y1(y), y2(y + height) {};
    std::vector<int> center() const;
    std::vector<std::vector<int>> inner(RectangularRoom &room) const;
};

class Map {
    public:
        int width, height;

        Map(int width, int height);
        bool is_wall(int x, int y) const;
        void render(tcod::Console& g_console) const;
        void dig(int x1, int y1, int x2, int y2);
        void dig_room(RectangularRoom &room);
    protected:
        std::vector<Tile> tiles;
        friend class BspListener;

        void create_room(std::vector<Actor>& actors, int x1, int y1, int x2, int y2);
        void set_can_walk(int x, int y, bool can_walk);
};
