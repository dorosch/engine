#ifndef __GRAPHICS_CAMERAS_EXCEPTION__
#define __GRAPHICS_CAMERAS_EXCEPTION__

#include "core/graphics/exception.hpp"

namespace Engine {
    namespace Graphics {
        namespace Camera {
            class UnsupportedCameraProjection : public GraphicsException {
                virtual const char* what() const throw() {
                    return "The selected projection is not supported by the camera";
                }
            };
        }
    }
}

#endif
