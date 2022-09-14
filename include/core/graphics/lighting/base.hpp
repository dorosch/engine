#ifndef __GRAPHICS_LIGHTING__
#define __GRAPHICS_LIGHTING__

#include "core/object/object.hpp"
#include "core/ecs/component.hpp"
#include "core/graphics/lighting/type.hpp"


namespace Engine {
    namespace Graphics {
        namespace Lighting {
            class Light : public Object {
            public:
                Light() : Object() {};
                virtual ~Light() {};
            };


            class DirectionalLight : public Light {
            public:
                DirectionalLight() : Light() {
                    name = "Directional light";
                    light = std::make_unique<Ecs::Component::Light>(Type::DIRECTIONAL);
                    // light->lightType = Type::DIRECTIONAL;
                };
            };
        }
    }
}

#endif
