#include <fmt/format.h>

#include "meta.hpp"
#include "engine.hpp"


namespace Engine {
    EngineApplication::EngineApplication() {
        this->windowManager = new Manager::WindowManager();
        this->eventManager = new Manager::EventManager();
    }


    EngineApplication::~EngineApplication() {
        this->eventManager->Shutdown();
        this->windowManager->Shutdown();

        delete this->windowManager;
    }


    Engine::Engine(EngineApplication *application) {
        this->app = application;
    }


    Engine::~Engine() {
        this->app->Shutdown();

        delete this->app;
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

        // Initialize all managers and then initialize the application
        // since the application can change the settings of managers.
        this->app->windowManager->Init();
        this->app->Init();
    }


    void Engine::Run() {
        /**
         * Main engine loop.
         * 
         * As long as the application window is open, engine 
         * continues updating user application and all managers. 
         */

        this->app->windowManager->window->Create();
        this->app->eventManager->Init(this->app->windowManager->window);

        while (this->app->windowManager->window->IsOpen()) {
            this->app->Update();
            this->app->windowManager->Update();
            this->app->eventManager->Update();
        }
    }
}
