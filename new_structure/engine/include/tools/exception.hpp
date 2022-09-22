#ifndef __RADIAN_TOOL_EXCEPTION__
#define __RADIAN_TOOL_EXCEPTION__

#include "radian.hpp"

namespace Radian {
    namespace Tool {
        class ToolException : public Exception {
        public:
            virtual ~ToolException() {}
            virtual const char* what() const noexcept = 0;
        };
    }
}

#endif
