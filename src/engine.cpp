#include <SDL.h>
#include "engine.hpp"
#include "actor.hpp"

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

Engine::Engine() : map(80,45), compute_fov(true), fov_radius(10) {
    // set up parameters for console emulator

    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "Sublight";

    auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    // set up the actual engine variables
    g_console = tcod::Console{80, 40};

    params.console = g_console.get();

    g_context = tcod::Context(params);

    actors.push_back(Actor(40,25,"@",tcod::ColorRGB{255,255,255}));

    map.generate_dungeon(get_player());
}

Engine::~Engine() {
    // actors is a pointer so we kinda just delete all the actors' pointers before clearing
    actors.clear();
}

Actor& Engine::get_player() {
    return actors[0];
}

/// @brief Renders all actors
void Engine::render() {
    // main rendering func
    // literally just loops thru all actors
    g_console.clear();

    map.render(g_console);

    for (auto &a : actors) {
        if (map.is_in_fov(a.x, a.y))
            tcod::print(g_console, {a.x, a.y}, a.character, a.color, std::nullopt);
    }

    g_context.present(g_console);
}

/// @brief Updates all actors
void Engine::update() {
    // loops thru all actors and updates them
    for (auto &a : actors) {
        a.update(g_console, g_context);
    }
}

void Engine::main_loop() {
    // get player actor
    Actor& player = get_player();
    
    // handle input
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                std::exit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                TCOD_mouse_t key;
                tcod::sdl2::process_event(event, key);  // Convert a SDL key to a libtcod key event, to help port older code.
                int x, y = 0;
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                        x = player.x;
                        y = player.y + 1;

                        if (map.get_tile(x, y).can_walk && map.is_in_bounds(x, y)){
                            player.move(0,1);
                            compute_fov = true;
                        }
                        break;
                    case SDLK_RIGHT:
                        x = player.x + 1;
                        y = player.y;

                        if (map.get_tile(x, y).can_walk && map.is_in_bounds(x, y)){
                            player.move(1,0);
                            compute_fov = true;
                        }
                        break;
                    case SDLK_LEFT:
                        x = player.x - 1;
                        y = player.y;

                        if (map.get_tile(x, y).can_walk && map.is_in_bounds(x, y)){
                            player.move(-1,0);
                            compute_fov = true;
                        }
                        break;
                    case SDLK_UP:
                        x = player.x;
                        y = player.y - 1;

                        if (map.get_tile(x, y).can_walk && map.is_in_bounds(x, y)){
                            player.move(0,-1);
                            compute_fov = true;
                        }
                        break;
                }
        }
    }
    if (compute_fov) {
        map.compute_fov(player.x, player.y, fov_radius);
        compute_fov = false;
    }

    // begin render loop
    render();

    // update the tah
    update();
}
