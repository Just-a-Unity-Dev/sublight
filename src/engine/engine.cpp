#include "engine.hpp"
#include "actor.hpp"
#include <SDL.h>

/// Return the data directory.
auto get_data_dir() -> std::filesystem::path {
    static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
    while (!std::filesystem::exists(root_directory / "assets")) {
        // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
        root_directory /= "..";
        if (!std::filesystem::exists(root_directory)) {
        throw std::runtime_error("Could not find the data directory.");
        }
    }
    return root_directory / "assets";
};

Engine::Engine() {
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "Sublight";

    auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    g_console = tcod::Console{80, 40};

    params.console = g_console.get();

    g_context = tcod::Context(params);

    player = new Actor(40,25,"@",TCOD_white);
    actors.push_back(player);
    // map = new Map(80,45);
}

Engine::~Engine() {
    for (Actor* a : actors) {
       delete a;
    }
    actors.clear();
}
