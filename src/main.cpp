#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

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

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.

/// Game loop.
void main_loop() {
    // Rendering.
    g_console.clear();
    tcod::print(g_console, {0, 0}, "Hello World", TCOD_white, std::nullopt);
    g_context.present(g_console);

    // Handle input.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            std::exit(EXIT_SUCCESS);
            break;
        }
    }
}

/// Main program entry point.
int main(int argc, char** argv) {
    try {
        auto params = TCOD_ContextParams{};
        params.tcod_version = TCOD_COMPILEDVERSION;
        params.argc = argc;
        params.argv = argv;
        params.renderer_type = TCOD_RENDERER_SDL2;
        params.vsync = 1;
        params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
        params.window_title = "Libtcod Template Project";

        auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
        params.tileset = tileset.get();

        g_console = tcod::Console{80, 40};
        params.console = g_console.get();

        g_context = tcod::Context(params);

        while (true) main_loop();
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << "\n";
        throw;
    }
}
