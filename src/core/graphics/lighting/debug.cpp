#include "core/graphics/lighting/debug.hpp"

using namespace Engine::Graphics::Lighting;


DebugLight::DebugLight() {
    name = "Light";
    light = std::make_unique<Ecs::Component::Light>();
}
