#ifndef __ECS_COMPONENT__
#define __ECS_COMPONENT__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "core/math/type.hpp"
#include "core/render/shader/base.hpp"


namespace Engine {
    namespace Ecs {
        namespace Component {
            enum Type {
                TRANSFORM,
                MATERIAL,
                MESH
            };


            class Component {
            public:
                Type type;
            };


            class Transform : public Component {
            public:
                Type type = Type::TRANSFORM;

                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
            };


            class Material : public Component {
            public:
                Type type = Type::MATERIAL;

                std::unique_ptr<Render::ShaderProgram> shader;
            };


            class Mesh : public Component {
            public:
                Type type = Type::MESH;

                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;
            };
        }   
    }
}

#endif
