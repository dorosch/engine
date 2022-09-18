#ifndef __ECS_ENTITY__
#define __ECS_ENTITY__

#include <memory>
#include <string>
#include <vector>

#include "core/ecs/component.hpp"


namespace Engine {
    namespace Ecs {
        class Entity {
        public:
            std::string name;
            std::unique_ptr<Component::Transform> transform = std::make_unique<Component::Transform>();
            std::unique_ptr<Component::Material> material;
            std::vector<std::unique_ptr<Component::Mesh>> meshes;
            std::unique_ptr<Component::Light> light;
            std::unique_ptr<Component::Camera> camera;

            Entity() {};
            virtual ~Entity() {};

            inline bool HasComponent(const Component::Type type) {
                switch (type) {
                    case Component::Type::TRANSFORM:
                        return transform != nullptr;
                    case Component::Type::MATERIAL:
                        return material != nullptr;
                    case Component::Type::MESH:
                        return meshes.size() > 0;
                    case Component::Type::LIGHT:
                        return light != nullptr;
                    case Component::Type::CAMERA:
                        return camera != nullptr;
                    default:
                        // TODO: Add logger->error
                        return false;
                }
            }
        };
    }
}

#endif
