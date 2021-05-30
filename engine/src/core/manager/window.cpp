#include "core/manager/window.hpp"


namespace Engine {
    namespace Manager {
        WindowManager::WindowManager() {
            logger = new Tool::Logger::Logger("windowm");

            logger->trace("constructor");

            width = 800;
            height = 600;
            title = "Application";
            fullScreen = false;
            canResize = false;
        }


        WindowManager::~WindowManager() {
            logger->trace("destructor");

            delete this->logger;
        }


        void WindowManager::Init() {
            GLFWmonitor* primaryMonitor = nullptr;

            logger->debug("init");

            glfwInit();

            glfwWindowHint(GLFW_SAMPLES, 4);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            if (!this->canResize) {
                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
            }

            if (this->fullScreen) {
                primaryMonitor = glfwGetPrimaryMonitor();
            }

            id = glfwCreateWindow(
                this->width, this->height, this->title, primaryMonitor, nullptr
            );
            
            glfwMakeContextCurrent(this->id);
        }

        void WindowManager::Update() {
            glfwPollEvents();

            glfwSwapBuffers(this->id);
        }

        void WindowManager::Shutdown() {
            logger->debug("shutdown");

            glfwDestroyWindow(this->id);
        }

        bool WindowManager::IsOpen() {
            return !glfwWindowShouldClose(this->id);
        }
    }
}
