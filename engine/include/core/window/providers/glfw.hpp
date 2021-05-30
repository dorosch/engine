#ifndef __GLFW_HPP__
#define __GLFW_HPP__

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tools/logger.hpp"
#include "core/window/provider.hpp"


namespace Engine {
    namespace Window {
        class GLFWProvider : public WindowProvider {
        private:
            GLFWmonitor* primaryMonitor = nullptr;
            Tool::Logger::Logger *logger;

        public:
            GLFWwindow  *object = nullptr;

            GLFWProvider();
            ~GLFWProvider();

            void Init();
            void Create();
            void Update();
            void Shutdown();
            bool IsOpen();
        };
    }
}


#endif
