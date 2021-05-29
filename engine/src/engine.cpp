#include <fmt/format.h>

#include "meta.hpp"
#include "engine.hpp"


namespace Engine {
    EngineApplication::EngineApplication() {
        this->window = nullptr;
    }


    EngineApplication::~EngineApplication() {
        delete this->window;
    }


    Engine::Engine(EngineApplication *application) {
        this->application = application;

        this->logger = new Tool::Logger::Logger("engine");
    }


    Engine::~Engine() {
        logger->info("engine shutdown");

        this->application->Shutdown();

        delete this->logger;
    }


    void Engine::Run() {
        /**
         * The main method of starting the engine.
         * 
         * The method initializes the user application and checks 
         * the correctness of the initialization. If one of the managers 
         * is not initialized, then the execution of the main loop 
         * will not start. 
         */

        logger->info(fmt::format("engine version: {}", ENGINE_VERSION).c_str());

        this->application->Init();

        if (!this->application->window) {
            return logger->critical(
                "The application must initialize WindowManager"
            );
        }

        while (this->application->window->IsOpen) {
            this->application->Update();
        }
    }
}
