#include <libtcod.hpp>
#include "map.hpp"

Map::Map(int width, int height) : width(width), height(height), tiles(width*height) {
    setWall(30,22);
    setWall(50,22);
};

bool Map::isWall(int x, int y) const {
    return !tiles[x+y*width].canWalk;
}

void Map::setWall(int x, int y) {
    Tile& tile = tiles[x+y*width];
    tile.canWalk = !tile.canWalk;
}

void Map::render(tcod::Console& g_console) const {
    static const TCOD_color_t darkWall = TCOD_color_RGB(0,0,100);
    static const TCOD_color_t darkGround = TCOD_color_RGB(50,50,150);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            tcod::print(g_console, {x, y}, isWall(x, y) ? "▓" : ".", isWall(x, y) ? darkWall : darkGround, std::nullopt);
        }
    }
}
