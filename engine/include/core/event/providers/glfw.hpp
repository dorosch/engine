#ifndef __GLFW_EVENT_PROVIDER_HPP__
#define __GLFW_EVENT_PROVIDER_HPP__

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/event/provider.hpp"
#include "tools/logger.hpp"

using namespace Tool::Logger;


namespace Engine {
    namespace Event {
        class GLFWEventProvider : public EventProvider {
        protected:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("glfwe");

        public:
            void MouseEventCallback() {};
            void KeyboardEventCallback() {};
            void MouseEventCallback(GLFWwindow *, int, int, int);
            void KeyboardEventCallback(GLFWwindow *, int, int, int, int);
            static void MouseEventCallbackStatic(GLFWwindow *, int, int, int);
            static void KeyboardEventCallbackStatic(GLFWwindow *, int, int, int, int);
        };
    }
}


#endif
