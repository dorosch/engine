#include "core/window/glfw.hpp"

using namespace Engine::Window;


void GLFWWindowProvider::Startup() {
    this->logger->trace(std::string("Startup"));

    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}


void GLFWWindowProvider::Create() {
    logger->trace(std::string("Create"));

    if (!this->settings.canResize) {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }

    if (this->settings.fullScreen) {
        this->primaryMonitor = glfwGetPrimaryMonitor();
    }

    this->object = glfwCreateWindow(
        this->settings.width,
        this->settings.height,
        this->settings.title.c_str(),
        this->primaryMonitor,
        nullptr
    );

    if (!this->object) {
        throw std::runtime_error("Window creation error");
    }

    glfwMakeContextCurrent(this->object);
    // glfwSetFramebufferSizeCallback(this->object, framebufferChangeSizeCallback);

    // static void framebufferChangeSizeCallback(GLFWwindow* window, int width, int height) {
    //     glViewport(0, 0, width, height);
    // }
}


void GLFWWindowProvider::Update() {
    glfwSwapBuffers(this->object);
}


void GLFWWindowProvider::Shutdown() {
    this->logger->trace(std::string("Shutdown"));

    glfwDestroyWindow(this->object);
}


bool GLFWWindowProvider::IsOpen() {
    return !glfwWindowShouldClose(this->object);
}
