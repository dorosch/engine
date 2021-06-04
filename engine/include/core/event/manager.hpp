#ifndef __EVENT_MANAGER_HPP__
#define __EVENT_MANAGER_HPP__

#include <memory>

#include "core/manager.hpp"
#include "core/window/provider.hpp"
#include "core/window/providers/glfw.hpp"
#include "core/event/provider.hpp"
#include "core/event/providers/glfw.hpp"
#include "tools/logger.hpp"

using namespace Tool::Logger;


namespace Engine {
    namespace Manager {
        class EventManager: public EngineManager {
            /**
             * The event manager is responsible for choosing an event 
             * provider and then calling the event provider update. 
             */

        private:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("evtm");

            Window::WindowProvider *window = nullptr;
            Event::EventProvider *provider = nullptr;

        public:
            void Init() {};
            void Init(Window::WindowProvider *);
            void Update();
            void Shutdown();
        };
    }
}


#endif
