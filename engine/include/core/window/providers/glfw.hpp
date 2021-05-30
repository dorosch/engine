#ifndef __GLFW_HPP__
#define __GLFW_HPP__

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tools/logger.hpp"
#include "core/window/provider.hpp"

using namespace Tool::Logger;


namespace Engine {
    namespace Window {
        class GLFWProvider : public WindowProvider {
        private:
            GLFWmonitor* primaryMonitor = nullptr;
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("glfw");

        public:
            GLFWwindow  *object = nullptr;

            void Init();
            void Create();
            void Update();
            void Shutdown();
            bool IsOpen();
        };
    }
}


#endif
