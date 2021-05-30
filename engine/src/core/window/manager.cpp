#include "core/window/manager.hpp"


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
            if (!this->provider) {
                this->provider = Window::Provider::GLFW;
            }

            switch (this->provider) {
                case Window::Provider::GLFW:
                    this->window = new Window::GLFWProvider();
                    break;
                default:
                    throw std::logic_error("Undefined WindowProvider");
            }

            this->window->Init();
        }

        void WindowManager::Update() {
            this->window->Update();
        }

        void WindowManager::Shutdown() {
            this->window->Shutdown();
        }
    }
}
