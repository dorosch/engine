#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include "core/manager/base.hpp"
#include "tools/logger.hpp"


namespace Engine {
    namespace Manager {
        class WindowManager: public EngineManager {
        private:
            Tool::Logger::Logger *logger;

        public:
            bool IsOpen = false;

            WindowManager();

            ~WindowManager();

            void Init();

            void Update();

            void Shutdown();
        };
    }
}


#endif
