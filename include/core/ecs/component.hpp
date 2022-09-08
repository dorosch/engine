#ifndef __ECS_COMPONENT__
#define __ECS_COMPONENT__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "core/math/type.hpp"
#include "core/render/buffer/base.hpp"
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

                glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 rotation = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
            };


            class Material : public Component {
            public:
                Type type = Type::MATERIAL;
            };


            class Mesh : public Component {
            public:
                Type type = Type::MESH;
                std::vector<float> vertices;
                std::vector<uint32_t> indices;
                std::unique_ptr<Render::VertexArray> VAO;
                std::unique_ptr<Render::VertexBuffer> VBO;
                std::unique_ptr<Render::IndexBuffer> EBO;

                /*
                 * Mesh initialization places the vertex data on the GPU and 
                 * places it there. Temporary solution for creating a mesh.
                 */
                void Initialize();
            };
        }   
    }
}

#endif
