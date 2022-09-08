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

                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
            };


            class Material : public Component {
            public:
                Type type = Type::MATERIAL;
            };


            class Mesh : public Component {
            public:
                Type type = Type::MESH;
                // float *vertices;
                // uint32_t *indices;
                // float vertices[] = {
                //     0.5f,  0.5f, 0.0f,  // Top Right
                //     0.5f, -0.5f, 0.0f,  // Bottom Right
                //     -0.5f, -0.5f, 0.0f,  // Bottom Left
                //     -0.5f,  0.5f, 0.0f   // Top Left 
                // };
                // uint32_t indices[] = {
                //     0, 1, 3,  // First Triangle
                //     1, 2, 3   // Second Triangle
                // };
                std::unique_ptr<Render::VertexArray> VAO;
                std::unique_ptr<Render::VertexBuffer> VBO;
                std::unique_ptr<Render::IndexBuffer> EBO;

                Mesh();
            };
        }   
    }
}

#endif
