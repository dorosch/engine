#ifndef __ECS_COMPONENT__
#define __ECS_COMPONENT__

#include <memory>

#include <glm/glm.hpp>

#include "core/render/shader/base.hpp"


namespace Engine {
    namespace Scene {
        namespace Component {
            enum Type {
                TRANSFORM,
                MATERIAL
            };


            class Component {
            public:
                Type type;
            };


            class Transform : public Component {
            public:
                Type type = Type::TRANSFORM;

                glm::vec3 position;
                glm::vec3 rotation;
                glm::vec3 scale;
            };


            class Material : public Component {
            public:
                Type type = Type::MATERIAL;

                std::unique_ptr<Render::ShaderProgram> shader;
            };
        }   
    }
}

#endif
