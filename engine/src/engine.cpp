#include <fmt/format.h>

#include "meta.hpp"
#include "engine.hpp"


namespace Engine {
    Engine::Engine(EngineApplication *application) {
        this->application = application;

        this->logger = new Logger::Logger("engine");
    }


    Engine::~Engine() {
        logger->info("engine shutdown");

        this->application->Shutdown();

        delete this->logger;
    }


    void Engine::Run() {
        logger->info("engine starting");
        logger->info(fmt::format("engine version: {}", ENGINE_VERSION).c_str());

        this->application->Run();

        while (true) {
            this->application->Update();
        }
    }
}
