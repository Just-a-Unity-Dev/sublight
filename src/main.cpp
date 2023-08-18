#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#include "entity.hpp"
#include "engine.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

Engine engine;

/// Game loop.
void main_loop() {
    // Rendering.
    g_console.clear();
    tcod::print(g_console, {0, 0}, "hi world", TCOD_white, std::nullopt);
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
int main() {
    try {
        

        while (true) main_loop();
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << "\n";
        throw;
    }
}
