#include "engine.hpp"
#include "editor/editor.hpp"


namespace Engine {
    namespace Editor {
        EngineEditor::EngineEditor() {
            logger->trace("constructor");
        }


        void EngineEditor::Init(EngineApplication *app) {
            logger->trace("Init");

            this->app = app;

            logger->info(this->app->windowManager->window->settings.title);
        }


        void EngineEditor::Update() {
            logger->trace("Update");
        }


        void EngineEditor::Shutdown() {
            logger->trace("Shutdown");
        }
    }
}
