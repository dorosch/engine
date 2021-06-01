#include "core/event/providers/glfw.hpp"


namespace Engine {
    namespace Event {
        void GLFWEventProvider::MouseEventCallbackStatic(
            GLFWwindow *window, int button, int action, int mods
        ) {
            static_cast<GLFWEventProvider*>(glfwGetWindowUserPointer(window))->
                MouseEventCallback(window, button, action, mods);
        }


        void GLFWEventProvider::KeyboardEventCallbackStatic(
            GLFWwindow *window, int key, int scancode, int action, int mods
        ) {
            static_cast<GLFWEventProvider*>(glfwGetWindowUserPointer(window))->
                KeyboardEventCallback(window, key, scancode, action, mods);
        }


        void GLFWEventProvider::MouseEventCallback(
            GLFWwindow *window, int button, int action, int mods
        ) {
            logger->debug("Mouse event");

            EventObserver::GetInstance()->Publish(
                new Event(EventType::Mouse)
            );
        }


        void GLFWEventProvider::KeyboardEventCallback(
            GLFWwindow *window, int key, int scancode, int action, int mods
        ) {
            logger->debug("Keyboard event");

            EventObserver::GetInstance()->Publish(
                new Event(EventType::Keyboard)
            );
        }
    }
}
