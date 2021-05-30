#include "engine.hpp"
#include "tools/logger.hpp"


class Application : public Engine::EngineApplication {
public:
    void Init() {
        logger->debug("init");

        windowManager->window->settings.canResize = true;
        windowManager->window->settings.title = "Application";
    }

    void Update() {}

    void Shutdown() {
        logger->debug("shutdown");
    }
};


int main() {
    Application *application = new Application();
    Engine::Engine engine(application);

    engine.Init();

    try {
        engine.Run();
    } catch (const std::exception& error) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
