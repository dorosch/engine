#include "engine.hpp"


int main() {
    Engine *engine = new Engine();

    try {
        engine->Start();
    } catch (const std::exception& e) {
        delete engine;

        std::cerr << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    delete engine;

    return EXIT_SUCCESS;
}
