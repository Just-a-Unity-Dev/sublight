#include <libtcod.hpp>
#include "actor.hpp"
#include "map.hpp"
#include "point2.hpp"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;

Map::Map(int width, int height) : width(width), height(height), tiles(width*height) {};


Point2 RectangularRoom::center() const {
    int center_x = int((x1 + x2) / 2);
    int center_y = int((y1 + y2) / 2);

    return Point2 {center_x, center_y};
}

std::vector<Point2> RectangularRoom::inner(RectangularRoom &room) const {
    std::vector<Point2> inner {};

    for (int x = room.x1 + 1; x < room.x2; ++x)
    {
        for (int y = room.y1 + 1; y < room.y2; ++y)
        {
            inner.push_back(Point2 {x, y});
        }
    }

    return inner;
}

bool Map::is_wall(int x, int y) const {
    return !tiles[x+y*width].can_walk;
}

void Map::set_tile(int x, int y, Tile new_tile) {
    tiles[x+y*width] = new_tile;
}

void Map::set_can_walk(int x, int y, bool can_walk) {
    tiles[x+y*width].can_walk = can_walk;
}

Tile& Map::get_tile(int x, int y) {
    return tiles[x+y*width];
}

void Map::dig(int x1, int y1, int x2, int y2) {
    if ( x2 < x1 ) {
        int tmp=x2;
        x2=x1;
        x1=tmp;
    }

    if ( y2 < y1 ) {
        int tmp=y2;
        y2=y1;
        y1=tmp;
    }

    for (int tile_x=x1; tile_x <= x2; tile_x++) {
        for (int tile_y=y1; tile_y <= y2; tile_y++) {
            set_can_walk(tile_x, tile_y, true);
        }
    }
}

void Map::dig_room(RectangularRoom &room) {
    for (int x = room.x1 + 1; x < room.x2; ++x)
    {
        for (int y = room.y1 + 1; y < room.y2; ++y)
        {
            set_can_walk(x, y, true);
        }
    }
}

std::vector<Point2> Map::tunnel_between(RectangularRoom &room1, RectangularRoom &room2) {
    std::vector<Point2> iterator;

    int x1 = room1.center().x;
    int y1 = room1.center().y;
    int x2 = room2.center().x;
    int y2 = room2.center().y;

    int corner_x = x2; // move horizontally then vertically
    int corner_y = y1;

    TCODRandom* random = TCODRandom::getInstance();
    if (random->getFloat(0, 1) < .5) { // 50% chance
        corner_x = x1; // move vertically then horizontally
        corner_y = y2;
    }

    tcod::BresenhamLine line = tcod::BresenhamLine(
        tcod::BresenhamLine::Point2(std::array<int, 2UI64> {x1, y1}),
        tcod::BresenhamLine::Point2(std::array<int, 2UI64> {corner_x, corner_y})
    );

    for (auto point : line) {
        iterator.push_back(Point2 {point[0], point[1]});
    }

    tcod::BresenhamLine line2 = tcod::BresenhamLine(
        tcod::BresenhamLine::Point2(std::array<int, 2UI64> {corner_x, corner_y}),
        tcod::BresenhamLine::Point2(std::array<int, 2UI64> {x2, y2})
    );

    for (auto point : line2) {
        iterator.push_back(Point2 {point[0], point[1]});
    }

    return iterator;
}

void Map::generate_dungeon() {
    RectangularRoom room = RectangularRoom(10, 10, 20, 20);
    RectangularRoom room2 = RectangularRoom(35, 10, 20, 20);
    dig_room(room);
    dig_room(room2);

    for (auto point : tunnel_between(room, room2)) {
        set_can_walk(point.x, point.y, true);
    }
}

void Map::render(tcod::Console& g_console) const {
    static const TCOD_color_t darkWall = TCOD_color_RGB(0,0,100);
    static const TCOD_color_t darkGround = TCOD_color_RGB(50,50,150);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            tcod::print(
                g_console,
                {x, y},
                is_wall(x, y) ? "▓" : ".",
                is_wall(x, y) ? darkWall : darkGround,
                std::nullopt
            );
        }
    }
}
