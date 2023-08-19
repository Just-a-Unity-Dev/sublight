#pragma once
#include <libtcod.hpp>
#include "actor.hpp"
#include "point2.hpp"
#include "tiles.hpp"

struct RectangularRoom {
    int x1;
    int x2;
    int y1;
    int y2;

    RectangularRoom(int x, int y, int width, int height) : x1(x), x2(x + width), y1(y), y2(y + height) {};
    Point2 center() const;
    std::vector<Point2> inner(RectangularRoom &room) const;
};

class Map {
    public:
        int width, height;

        Map(int width, int height);
        bool is_wall(int x, int y) const;
        void render(tcod::Console& g_console) const;
        void dig(int x1, int y1, int x2, int y2);
        void dig_room(RectangularRoom &room);
        void generate_dungeon();
        Tile& get_tile(int x, int y);
        std::vector<Point2> tunnel_between(RectangularRoom &room1, RectangularRoom &room2);
    protected:
        std::vector<Tile> tiles;
        friend class BspListener;

        void set_can_walk(int x, int y, bool can_walk);
        void set_tile(int x, int y, Tile new_tile);
};
