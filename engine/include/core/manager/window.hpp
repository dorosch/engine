#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include "core/manager/base.hpp"


namespace Engine {
    namespace Manager {
        class WindowManager: public EngineManager {
        public:
            bool IsOpen = true;
        };
    }
}


#endif
