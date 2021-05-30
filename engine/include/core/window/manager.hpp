#ifndef __WINDOW_MANAGER_HPP__
#define __WINDOW_MANAGER_HPP__

#include <memory>

#include "core/manager.hpp"
#include "core/window/provider.hpp"
#include "core/window/providers/glfw.hpp"
#include "tools/logger.hpp"

using namespace Tool::Logger;


namespace Engine {
    namespace Manager {
        class WindowManager: public EngineManager {
        private:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("winm");

        public:
            Window::Provider provider;
            Window::WindowProvider *window;

            void Init();
            void Update();
            void Shutdown();
        };
    }
}


#endif
