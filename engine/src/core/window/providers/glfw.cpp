#include "core/window/providers/glfw.hpp"


namespace Engine {
    namespace Window {
        void GLFWWindowProvider::Init() {
            logger->debug("init");

            glfwInit();

            glfwWindowHint(GLFW_SAMPLES, 4);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        }


        void GLFWWindowProvider::Create() {
            logger->debug("create");

            if (!this->settings.canResize) {
                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
            }

            if (this->settings.fullScreen) {
                primaryMonitor = glfwGetPrimaryMonitor();
            }

            this->object = glfwCreateWindow(
                this->settings.width,
                this->settings.height,
                this->settings.title,
                this->primaryMonitor, nullptr
            );

            if (!this->object) {
                logger->critical("Window creation error");
                throw std::runtime_error("Window creation error");
            }

            glfwMakeContextCurrent(this->object);
        }


        void GLFWWindowProvider::Update() {
            glfwSwapBuffers(this->object);
        }


        void GLFWWindowProvider::Shutdown() {
            logger->debug("shutdown");

            glfwDestroyWindow(this->object);
        }


        bool GLFWWindowProvider::IsOpen() {
            return !glfwWindowShouldClose(this->object);
        }
    }
}
