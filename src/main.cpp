#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#include "engine.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

Engine engine;

/// Main program entry point.
int main() {
    try {
        while (true)
            engine.main_loop();
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << "\n";
        throw;
    }
}
