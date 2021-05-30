#include <fmt/format.h>

#include "meta.hpp"
#include "engine.hpp"


namespace Engine {
    EngineApplication::EngineApplication() {
        this->window = new Manager::WindowManager();

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
        this->app = application;

        logger = new Tool::Logger::Logger("engine");

        logger->trace("constructor");
    }


    Engine::~Engine() {
        logger->trace("destructor");

        this->app->Shutdown();

        delete this->app;
        delete this->logger;
    }


    void Engine::Init() {
        /**
         * Engine initialization.
         * 
         * At this stage, the engine initializes the user application 
         * and all managers. Also at this stage, it is possible to check 
         * for the initialization of managers. 
         */

        logger->info(fmt::format("engine version: {}", ENGINE_VERSION).c_str());

        this->app->Init();
        this->app->window->Init();
    }


    void Engine::Run() {
        /**
         * Main engine loop.
         * 
         * As long as the application window is open, engine 
         * continues updating user application and all managers. 
         */

        while (this->app->window->IsOpen()) {
            this->app->Update();
            this->app->window->Update();
        }
    }
}
