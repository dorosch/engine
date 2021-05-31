#include "core/event/manager.hpp"


namespace Engine {
    namespace Manager {
        void EventManager::Init(Window::WindowProvider *window) {
            logger->debug("init");

            this->window = window;
        }

        void EventManager::Update() {

        }

        void EventManager::Shutdown() {
            logger->debug("shutdown");
        }
    }
}
