#include "engine.hpp"
#include "core/manager/window.hpp"
#include "tools/logger.hpp"


class Application : public Engine::EngineApplication {
private:
    Tool::Logger::Logger *logger;

public:
    void Init() {
        logger = new Tool::Logger::Logger("app");

        logger->info("application init");

        window = new Engine::Manager::WindowManager();
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
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
