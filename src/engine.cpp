#include <SDL.h>
#include "engine.hpp"
#include "actors.hpp"

static const int ROOM_MAX_SIZE = 10;
static const int ROOM_MIN_SIZE = 6;
static const int ROOM_MAX_AMOUNT = 15;
static const int MAX_MONSTERS_PER_ROOM = 2;

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

    map.actors.push_back(actordefs::player);

    map.generate_dungeon(get_player(), ROOM_MAX_AMOUNT, ROOM_MIN_SIZE, ROOM_MAX_SIZE, MAX_MONSTERS_PER_ROOM);

    handler = InputHandler();
}

Engine::~Engine() {
    // actors is a pointer so we kinda just delete all the actors' pointers before clearing
    map.actors.clear();
}

Actor& Engine::get_player() {
    return map.actors[0];
}

/// @brief Renders all actors
void Engine::render() {
    // main rendering func
    // literally just loops thru all actors
    g_console.clear();

    map.render(g_console);

    g_context.present(g_console);
}

/// @brief Updates all actors
void Engine::update() {
    // loops thru all actors and updates them
    for (auto &a : map.actors) {
        a.update(g_console, g_context);
    }
}

void InputHandler::bump_action(int dx, int dy) {
    Actor& player = engine.get_player();

    if (engine.map.get_blocking_actor_at_position(player.x + dx, player.y + dy).x != -1) {
        attack_action(dx, dy);
    } else {
        move_action(dx, dy);
    }
}

void InputHandler::move_action(int dx, int dy) {
    Actor& player = engine.get_player();

    if (!engine.map.is_obstructed(player.x + dx, player.y + dy)){
        player.move(dx,dy);
    }
}

void InputHandler::attack_action(int dx, int dy) {
    Actor& player = engine.get_player();
    Actor attacked = engine.map.get_blocking_actor_at_position(player.x + dx, player.y + dy);

    printf(attacked.name);
    if (attacked.x == -1) {
        // nothing
        return;
    }
}

void InputHandler::toggle_combat_action() {
    engine.combat_mode = !engine.combat_mode;
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
                        engine.handler.bump_action(0, 1);
                        compute_fov = true;
                        break;
                    case SDLK_RIGHT:
                        engine.handler.bump_action(1, 0);
                        compute_fov = true;
                        break;
                    case SDLK_LEFT:
                        engine.handler.bump_action(-1, 0);
                        compute_fov = true;
                        break;
                    case SDLK_UP:
                        engine.handler.bump_action(0, -1);
                        compute_fov = true;
                        break;
                    case SDLK_t:
                        engine.handler.toggle_combat_action();
                        compute_fov = true;
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
