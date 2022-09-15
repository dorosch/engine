#ifndef __ECS_COMPONENT__
#define __ECS_COMPONENT__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "core/math/type.hpp"
#include "core/graphics/lighting/type.hpp"
#include "core/render/buffer/base.hpp"
#include "core/render/shader/base.hpp"


namespace Engine {
    namespace Ecs {
        namespace Component {
            enum Type {
                TRANSFORM,
                MATERIAL,
                MESH,
                LIGHT
            };


            class Component {
            public:
                Type type;
            };


            class Transform : public Component {
            public:
                const Type type = Type::TRANSFORM;

                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 1.0f);
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
            };


            class Material : public Component {
            public:
                const Type type = Type::MATERIAL;

                float shininess = 10;
                glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
            };


            class Mesh : public Component {
            public:
                const Type type = Type::MESH;

                std::vector<float> vertices;
                std::unique_ptr<Render::VertexArray> VAO;
                std::unique_ptr<Render::VertexBuffer> VBO;

                /*
                 * Mesh initialization places the vertex data on the GPU and 
                 * places it there. Temporary solution for creating a mesh.
                 */
                void Initialize();
            };


            class Light : public Component {
            public:
                const Type type = Type::LIGHT;
                Graphics::Lighting::Type lightType;

                float intensity = 1.0f;
                glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
                glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
                glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

                // Only for directional and spot lighting
                glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

                // Only for point and spot lighting
                float constant = 1.0f;
                float linear = 0.35f;
                float quadratic = 0.44f;

                // Only for spot lighting
                float cutOff = 12.5f;
                float outerCutOff = 17.5f;

                Light(Graphics::Lighting::Type type) : lightType(type) {};
            };
        }
    }
}

#endif
