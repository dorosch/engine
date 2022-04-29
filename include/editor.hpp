#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tools/logger.hpp"

using namespace Tool::Logger;


namespace Engine {
    // class Application;


    namespace Editor {
        class Editor {
        private:
            GLFWwindow *window = nullptr;

        public:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("editor");

            void Startup(GLFWwindow *);
            void Update();
            void Shutdown();
        };
    }
}

#endif
