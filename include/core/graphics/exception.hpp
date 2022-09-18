#ifndef __GRAPHICS_EXCEPTION__
#define __GRAPHICS_EXCEPTION__

#include <exception>

namespace Engine {
    namespace Graphics {
        class GraphicsException : public std::exception {
        public:
            virtual ~GraphicsException() {}
            virtual const char* what() const noexcept = 0;
        };
    }
}

#endif
