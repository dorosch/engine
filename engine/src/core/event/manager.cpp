#include "core/event/manager.hpp"


namespace Engine {
    namespace Manager {
        void EventManager::Init(Window::WindowProvider *window) {
            logger->debug("init");

            this->window = window;

            switch (this->window->provider) {
                case Window::Provider::GLFW:
                    this->provider = new Event::GLFWEventProvider();

                    glfwSetWindowUserPointer(
                        static_cast<Window::GLFWWindowProvider*>(this->window)->object,
                        static_cast<Event::GLFWEventProvider*>(this->provider)
                    );

                    glfwSetKeyCallback(
                        static_cast<Window::GLFWWindowProvider*>(this->window)->object,
                        static_cast<Event::GLFWEventProvider*>(this->provider)->KeyboardEventCallbackStatic
                    );

                    glfwSetMouseButtonCallback(
                        static_cast<Window::GLFWWindowProvider*>(this->window)->object,
                        static_cast<Event::GLFWEventProvider*>(this->provider)->MouseEventCallbackStatic
                    );

                    break;
                default:
                    throw std::logic_error("Undefined EventProvider");
            }
        }

        void EventManager::Update() {
            glfwPollEvents();
        }

        void EventManager::Shutdown() {
            logger->debug("shutdown");
        }
    }
}
