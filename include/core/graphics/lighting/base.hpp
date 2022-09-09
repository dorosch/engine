#ifndef __GRAPHICS_LIGHTING_DEBUG__
#define __GRAPHICS_LIGHTING_DEBUG__

#include "core/ecs/component.hpp"
#include "core/geometry/primitives.hpp"


namespace Engine {
    namespace Graphics {
        namespace Lighting {
            class Light : public Geometry::Cube {
            public:
                Light();
            };
        }
    }
}

#endif
