#include <libtcod.hpp>
#include "actor.hpp"
#include "actors.hpp"
#include "map.hpp"
#include "point2.hpp"

Map::Map(int width, int height) : width(width), height(height), tiles(width*height) {
    map = new TCODMap(width, height);
};

Map::~Map() {
    delete map;
}

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

bool RectangularRoom::intersects(RectangularRoom &other) const {
    return (
        x1 <= other.x2 &&
        x2 >= other.x1 &&
        y1 <= other.y2 &&
        y2 >= other.x1
    );
}

void Map::place_entities(RectangularRoom& room, int max_monsters) {
    TCODRandom* random = TCODRandom::getInstance();
    int monster_amount = random->getInt(0, max_monsters);

    for (int i = 0; i < monster_amount; ++i) {
        int x = random->getInt(room.x1 + 1, room.x2 - 1);
        int y = random->getInt(room.y1 + 1, room.y2 - 1);

        if (is_obstructed(x, y)) {
            // spawn a monster here
            Actor monster = actordefs::human;
            monster.x = x;
            monster.y = y;
            actors.push_back(monster);
        }
    }
}

bool Map::is_wall(int x, int y) const {
    return !tiles[x+y*width].can_walk;
}

bool Map::is_in_fov(int x, int y) const {
    return map->isInFov(x, y);
}

bool Map::is_in_bounds(int x, int y) const {
    return 0 <= x < width && 0 <= y < height;
}

bool Map::is_obstructed(int x, int y) const {
    if (is_wall(x, y)) {
        return false;
    }
    if (!is_in_bounds(x, y)) {
        return false;
    }
    for (auto actor : actors) {
        if (actor.x == x && actor.y == y && actor.blocks_movement) {
            return false;
        }
    }
    return true;
}

void Map::set_tile(
    int x,
    int y,
    Tile tile
) {
    tiles[x+y*width] = tile;
    map->setProperties(x,y,tile.transparent,tile.can_walk);
}

Tile& Map::get_tile(int x, int y) {
    return tiles[x+y*width];
}

void Map::dig_room(RectangularRoom &room) {
    for (int x = room.x1 + 1; x < room.x2; ++x)
    {
        for (int y = room.y1 + 1; y < room.y2; ++y)
        {
            set_tile(x, y, tiledefs::floor);
        }
    }
}

void Map::compute_fov(int x, int y, int radius) {
    map->computeFov(x, y, radius);
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

/// Generates a dungeon.
void Map::generate_dungeon(Actor& player, int room_max_amount, int room_min_size, int room_max_size, int max_monsters_per_room) {
    std::vector<RectangularRoom> rooms;
    TCODRandom* random = TCODRandom::getInstance();

    for (int i = 0; i < room_max_amount; ++i) {
        int room_width = random->getInt(room_min_size, room_max_size);
        int room_height = random->getInt(room_min_size, room_max_size);

        int room_x = random->getInt(0, width - room_width - 1);
        int room_y = random->getInt(0, height - room_height - 1);

        RectangularRoom room = RectangularRoom(room_x, room_y, room_width, room_height);

        bool intersected = false;
        for (auto other : rooms) {
            if (room.intersects(other)) {
                // intersects with another room, skip
                intersected = true;
                break;
            }
        }
        if (intersected)
            continue;

        dig_room(room);

        if (rooms.size() == 0) {
            player.x = room.center().x;
            player.y = room.center().y;
        } else {
            for (auto point : tunnel_between(room, rooms.back())) {
                set_tile(point.x, point.y, tiledefs::floor);
            }
        }

        place_entities(room, max_monsters_per_room);

        rooms.push_back(room);
    }
}

void Map::render(tcod::Console& g_console) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (is_in_fov(x, y)) {
                Tile tile = get_tile(x,y);

                tcod::print(
                    g_console,
                    {x, y},
                    tile.character,
                    tile.foreground,
                    tile.background
                );
            }
        }
    }

    for (auto &a : actors) {
        if (is_in_fov(a.x, a.y))
            tcod::print(g_console, {a.x, a.y}, a.character, a.color, std::nullopt);
    }
}
