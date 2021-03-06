#include <iostream>

#include "engine.hpp"
#include "tools/logger.hpp"


void event_callback(Engine::Event::Event *event) {
    std::cout << "New event in application" << std::endl;
}


class Application : public Engine::EngineApplication {
public:
    void Init() {
        logger->trace("Init");

        windowManager->window->settings.width = 1680;
        windowManager->window->settings.height = 1050;
        windowManager->window->settings.canResize = true;
        windowManager->window->settings.title = "Application";

        Engine::Event::EventObserver::GetInstance()->Subscribe(event_callback);
    }

    void Update() {}

    void Shutdown() {
        logger->trace("Shutdown");
    }
};


int main() {
    Application *application = new Application();
    Engine::Engine engine(application);

    engine.Init();

    try {
        engine.Run();
    } catch (const std::exception& error) {
        delete application;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}
