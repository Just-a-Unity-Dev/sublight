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

    player = new Actor(40,25,"@",TCOD_white);
    actors.push_back(player);
    // map = new Map(80,45);
}

Engine::~Engine() {
    // actors is a pointer so we kinda just delete all the actors' pointers before clearing
    for (Actor* a : actors) {
        delete a;
    }
    actors.clear();
}

/// @brief Renders all actors
void Engine::render() {
    // main rendering func
    // literally just loops thru all actors
    g_console.clear();
    for (Actor* a : actors) {
        tcod::print(g_console, {a->x, a->y}, a->character, a->color, std::nullopt);
    }
    g_context.present(g_console);
}

/// @brief Updates all actors
void Engine::update() {
    // loops thru all actors and updates them
    for (Actor* a : actors) {
        a->update(g_console, g_context);
    }
}

void Engine::main_loop() {
    // begin render loop
    render();

    // update the tah
    update();

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
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN: player->move(0,1); break;
                    case SDLK_RIGHT: player->move(1,0); break;
                    case SDLK_LEFT: player->move(-1,0); break;
                    case SDLK_UP: player->move(0,-1); break;
                }
        }
    }
}
