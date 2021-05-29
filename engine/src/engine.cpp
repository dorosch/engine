#include <fmt/format.h>

#include "meta.hpp"
#include "engine.hpp"


namespace Engine {
    EngineApplication::EngineApplication() {
        this->window = nullptr;

        logger = new Tool::Logger::Logger("app");

        logger->trace("constructor");
    }


    EngineApplication::~EngineApplication() {
        logger->trace("destructor");

        this->window->Shutdown();

        delete this->window;
        delete this->logger;
    }


    Engine::Engine(EngineApplication *application) {
        this->application = application;

        logger = new Tool::Logger::Logger("engine");

        logger->trace("constructor");
    }


    Engine::~Engine() {
        logger->trace("destructor");

        this->application->Shutdown();

        delete this->application;
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
        } else {
            this->application->window->Init();
        }

        while (this->application->window->IsOpen) {
            this->application->Update();
        }
    }
}
