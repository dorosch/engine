#include "engine.hpp"
#include "core/manager/window.hpp"
#include "tools/logger.hpp"


class Application : public Engine::EngineApplication {
public:
    void Init() {
        logger->debug("init");
    }

    void Update() {}

    void Shutdown() {
        logger->debug("shutdown");
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
