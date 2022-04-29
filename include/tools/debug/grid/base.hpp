#ifndef __DEBUG_GRID_BASE__
#define __DEBUG_GRID_BASE__

#include "tools/debug/base.hpp"


namespace Tool {
    namespace Debug {
        class DebugFloorGrid : public Debug {
        public:
            static DebugFloorGrid *GetInstance();
        };
    }
}

#endif
