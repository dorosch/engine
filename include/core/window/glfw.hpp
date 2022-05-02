#ifndef __WINDOW_GLFW__
#define __WINDOW_GLFW__

#include <string>

#include <GLFW/glfw3.h>

#include "core/window/base.hpp"
#include "core/window/glfw.hpp"


namespace Engine {
    namespace Window {
        class GLFWWindowProvider : public WindowProvider {
        private:
            GLFWmonitor *primaryMonitor = nullptr;

        public:
            // TODO: Return window object from virtual method with template
            GLFWwindow *object = nullptr;
            Provider provider = Provider::GLFW;

            void Startup();
            void Create();
            void Update();
            void Shutdown();
            bool IsOpen();
        };
    }
}

#endif
