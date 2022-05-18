#ifndef __ECS_COMPONENT__
#define __ECS_COMPONENT__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "core/render/type.hpp"
#include "core/render/shader/base.hpp"


namespace Engine {
    namespace Scene {
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

                glm::vec3 position;
                glm::vec3 rotation;
                glm::vec3 scale;
            };


            class Material : public Component {
            public:
                Type type = Type::MATERIAL;

                std::unique_ptr<Render::ShaderProgram> shader;
            };


            class Mesh : public Component {
            public:
                Type type = Type::MESH;
                std::string name;

                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;
            };


            // Standard meshes of primitives


            class Plane : public Mesh {
            public:
                Plane() {
                    name = std::string("Plane");

                    vertices = {
                        Vertex {
                            glm::vec3(0.5f,  0.5f, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f),
                            glm::vec2(1.0f, 1.0f)
                        },
                        Vertex {
                            glm::vec3(0.5f, -0.5f, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f),
                            glm::vec2(1.0f, 0.0f)
                        },
                        Vertex {
                            glm::vec3(-0.5f, -0.5f, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f),
                            glm::vec2(0.0f, 0.0f)
                        },
                        Vertex {
                            glm::vec3(-0.5f,  0.5f, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(1.0f, 1.0f, 0.0f),
                            glm::vec2(0.0f, 1.0f)
                        }
                    };
                    indices = {
                        0, 1, 3,
                        1, 2, 3
                    };
                }
            };
        }   
    }
}

#endif
