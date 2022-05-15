#ifndef __ECS_ENTITY__
#define __ECS_ENTITY__

#include <memory>
#include <string>

#include "core/scene/ecs/component.hpp"


namespace Engine {
    namespace Scene {
        class Entity {
        public:
            std::unique_ptr<Component::Transform> transform;
            std::unique_ptr<Component::Material> material;

            inline bool HasComponent(Component::Type type) {
                switch (type) {
                    case Component::Type::TRANSFORM:
                        return transform != nullptr;
                    case Component::Type::MATERIAL:
                        return material != nullptr;
                    default:
                        // TODO: Add logger->error
                        return false;
                }
            }
        };
    }
}

#endif
