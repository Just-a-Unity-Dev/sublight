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
    bool intersects(RectangularRoom &other) const;
};

class Map {
    public:
        std::vector<Actor> actors;
        int width, height;

        Map(int width, int height);
        ~Map();
        bool is_in_bounds(int x, int y) const;
        bool is_wall(int x, int y) const;
        bool is_obstructed(int x, int y) const;
        bool is_in_fov(int x, int y) const;

        void place_entities(RectangularRoom& room, int max_monsters);

        void render(tcod::Console& g_console);
        void dig(int x1, int y1, int x2, int y2);
        void dig_room(RectangularRoom &room);
        void generate_dungeon(Actor& player, int room_max_amount, int room_min_size, int room_max_size, int max_monsters_per_room);

        void compute_fov(int x, int y, int radius);

        Tile& get_tile(int x, int y);
        std::vector<Point2> tunnel_between(RectangularRoom &room1, RectangularRoom &room2);
    protected:
        std::vector<Tile> tiles;
        friend class BspListener;
        TCODMap *map;

        void set_can_walk(int x, int y, bool can_walk);
        void set_tile(
            int x,
            int y,
            Tile tile
        );
};
