#ifndef __DEBUG_BASE__
#define __DEBUG_BASE__

#include <glm/glm.hpp>


namespace Tool {
    namespace Debug {
        class Debug {
        public:
            virtual void Enable() = 0;
            virtual void SetMVP(glm::mat4) = 0;
        };
    }
}

#endif
