#include "core/manager/window.hpp"


namespace Engine {
    namespace Manager {
        WindowManager::WindowManager() {
            logger = new Tool::Logger::Logger("windowm");

            logger->trace("constructor");
        }


        WindowManager::~WindowManager() {
            logger->trace("destructor");

            delete this->logger;
        }


        void WindowManager::Init() {
            logger->debug("init");
        }

        // The window manager doesn't need to update in engine loop 
        void WindowManager::Update() {}

        void WindowManager::Shutdown() {
            logger->debug("shutdown");
        }
    }
}
