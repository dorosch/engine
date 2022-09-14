#ifndef __GRAPHICS_LIGHTING__
#define __GRAPHICS_LIGHTING__

#include "core/object/object.hpp"
#include "core/ecs/component.hpp"
#include "core/graphics/lighting/type.hpp"
#include "core/geometry/primitives.hpp"


namespace Engine {
    namespace Graphics {
        namespace Lighting {
            class Light : public Geometry::Cube {
            public:
                Light() : Cube() {};
                virtual ~Light() {};
            };


            class DirectionalLight : public Light {
            public:
                DirectionalLight() : Light() {
                    name = "Directional light";
                    light = std::make_unique<Ecs::Component::Light>(Type::DIRECTIONAL);
                };
            };


            class PointLight : public Light {
            public:
                PointLight() : Light() {
                    name = "Point light";
                    light = std::make_unique<Ecs::Component::Light>(Type::POINT);
                };
            };
        }
    }
}

#endif
