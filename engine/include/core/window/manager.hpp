#ifndef __WINDOW_MANAGER_HPP__
#define __WINDOW_MANAGER_HPP__

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/manager.hpp"
#include "tools/logger.hpp"


namespace Engine {
    namespace Manager {
        class WindowManager: public EngineManager {
        private:
            Tool::Logger::Logger *logger;

        public:
            GLFWwindow* id;
            int width;
            int height;
            const char *title;
            bool fullScreen;
            bool canResize;

            WindowManager();
            ~WindowManager();
            void Init();
            void Update();
            void Shutdown();
            bool IsOpen();
        };
    }
}


#endif
