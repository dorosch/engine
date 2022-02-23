#include <fmt/format.h>

#include "meta.hpp"
#include "engine.hpp"
#include "editor/editor.hpp"


namespace Engine {
    EngineApplication::EngineApplication() {
        logger->trace("constructor");

        this->windowManager = new Manager::WindowManager();
        this->eventManager = new Manager::EventManager();
        this->editor = new Editor::EngineEditor(this);
    }


    EngineApplication::~EngineApplication() {
        logger->trace("destructor");

        this->eventManager->Shutdown();
        this->windowManager->Shutdown();

        delete this->windowManager;
        delete this->eventManager;
        delete this->editor;
    }


    Engine::Engine(EngineApplication *application) {
        logger->trace("constructor");

        this->app = application;
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

        logger->trace("run");

        this->app->windowManager->window->Create();
        this->app->eventManager->Init(this->app->windowManager->window);
        this->app->editor->Init();

        while (this->app->windowManager->window->IsOpen()) {
            this->app->Update();
            this->app->editor->Update();
            this->app->eventManager->Update();
            this->app->windowManager->Update();
        }

        this->app->editor->Shutdown();
        this->app->Shutdown();
    }
}
