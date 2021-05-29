#include <iostream>

#include "engine.hpp"
#include "tools/logger.hpp"


class Application : public Engine::EngineApplication {
private:
    Engine::Logger::Logger *logger;

public:
    void Run() {
        logger = new Engine::Logger::Logger("app");

        logger->info("application starting");
    }

    void Update() {
        // TODO: Implement me
    }

    void Shutdown() {
        logger->info("application shutdown");

        delete logger;
    }
};


int main() {
    Application *application = new Application();
    Engine::Engine engine(application);

    try {
        engine.Run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
