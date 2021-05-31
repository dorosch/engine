#include "core/event/providers/glfw.hpp"


namespace Engine {
    namespace Event {
        void GLFWEventProvider::MouseEventCallbackStatic(
            GLFWwindow *window, int button, int action, int mods
        ) {
            // I don't like this solution with change glfwGetWindowUserPointer
            static_cast<GLFWEventProvider*>(glfwGetWindowUserPointer(window))->
                MouseEventCallback(window, button, action, mods);
        };


        void GLFWEventProvider::KeyboardEventCallbackStatic(
            GLFWwindow *window, int key, int scancode, int action, int mods
        ) {
            // I don't like this solution with change glfwGetWindowUserPointer
            static_cast<GLFWEventProvider*>(glfwGetWindowUserPointer(window))->
                KeyboardEventCallback(window, key, scancode, action, mods);
        };


        void GLFWEventProvider::MouseEventCallback(
            GLFWwindow *window, int button, int action, int mods
        ) {
            logger->info("Mouse event");
        }


        void GLFWEventProvider::KeyboardEventCallback(
            GLFWwindow *window, int key, int scancode, int action, int mods
        ) {
            logger->info("Keyboard event");
        }
    }
}
