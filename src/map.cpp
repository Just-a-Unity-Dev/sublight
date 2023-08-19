#include <libtcod.hpp>
#include "actor.hpp"
#include "map.hpp"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;

Map::Map(int width, int height) : width(width), height(height), tiles(width*height) {
    RectangularRoom room = RectangularRoom(15, 15, 20, 20);
    dig_room(room);
};


std::vector<int> RectangularRoom::center() const {
    int center_x = int((x1 + x2) / 2);
    int center_y = int((y1 + y2) / 2);

    std::vector<int> position;
    position.push_back(center_x);
    position.push_back(center_y);

    return position;
}
std::vector<std::vector<int>> RectangularRoom::inner(RectangularRoom &room) const {
    std::vector<std::vector<int>> inner {};

    for (int x = room.x1 + 1; x < room.x2; ++x)
    {
        for (int y = room.y1 + 1; y < room.y2; ++y)
        {
            inner.push_back(std::vector<int> {x, y});
        }
    }

    return inner;
}

bool Map::is_wall(int x, int y) const {
    return !tiles[x+y*width].canWalk;
}

void Map::set_can_walk(int x, int y, bool can_walk) {
    Tile& tile = tiles[x+y*width];
    tile.canWalk = !tile.canWalk;
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
    for (int x = room.x1; x < room.x2; ++x)
    {
        for (int y = room.y1; y < room.y2; ++y)
        {
            set_can_walk(x, y, true);
        }
    }

    for (int x = room.x1 + 1; x < room.x2 - 1; ++x)
    {
        for (int y = room.y1 + 1; y < room.y2 - 1; ++y)
        {
            set_can_walk(x, y, false);
        }
    }
}

void Map::create_room(std::vector<Actor>& actors, int x1, int y1, int x2, int y2) {}

void Map::render(tcod::Console& g_console) const {
    static const TCOD_color_t darkWall = TCOD_color_RGB(0,0,100);
    static const TCOD_color_t darkGround = TCOD_color_RGB(50,50,150);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            tcod::print(g_console, {x, y}, is_wall(x, y) ? "▓" : ".", is_wall(x, y) ? darkWall : darkGround, std::nullopt);
        }
    }
}
