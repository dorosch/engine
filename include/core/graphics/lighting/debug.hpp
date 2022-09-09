#ifndef __GRAPHICS_LIGHTING_DEBUG__
#define __GRAPHICS_LIGHTING_DEBUG__

#include "core/ecs/component.hpp"
#include "core/geometry/primitives.hpp"


namespace Engine {
    namespace Graphics {
        namespace Lighting {
            class DebugLight : public Geometry::Cube {
            public:
                DebugLight();
            };
        }
    }
}

#endif
