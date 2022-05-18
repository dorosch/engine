#ifndef __ECS_ENTITY__
#define __ECS_ENTITY__

#include <memory>
#include <string>

#include "core/scene/ecs/component.hpp"


namespace Engine {
    namespace Scene {
        class Entity {
        public:
            std::string name;
            std::unique_ptr<Component::Transform> transform = std::make_unique<Component::Transform>();
            std::unique_ptr<Component::Material> material;
            std::unique_ptr<Component::Mesh> mesh;

            Entity() {};
            virtual ~Entity() {};

            inline bool HasComponent(Component::Type type) {
                switch (type) {
                    case Component::Type::TRANSFORM:
                        return transform != nullptr;
                    case Component::Type::MATERIAL:
                        return material != nullptr;
                    case Component::Type::MESH:
                        return mesh != nullptr;
                    default:
                        // TODO: Add logger->error
                        return false;
                }
            }
        };
    }
}

#endif
