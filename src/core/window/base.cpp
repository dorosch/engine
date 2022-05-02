#include "core/window/base.hpp"
#include "core/window/glfw.hpp"

using namespace Engine::Window;


WindowProvider *WindowProvider::GetInstance() {
    // TODO: Pass application instance for switch window provider
    return new GLFWWindowProvider();
}
