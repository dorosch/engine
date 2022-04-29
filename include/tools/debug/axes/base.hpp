#ifndef __DEBUG_AXES_BASE__
#define __DEBUG_AXES_BASE__

#include "tools/debug/base.hpp"


namespace Tool {
    namespace Debug {
        class DebugAxes : public Debug {
        public:
            static DebugAxes *GetInstance();
        };
    }
}

#endif
