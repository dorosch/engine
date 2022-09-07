#ifndef __GEOMETRY_PRIMITIVE__
#define __GEOMETRY_PRIMITIVE__

#include <memory>

#include "core/math/type.hpp"
#include "core/ecs/component.hpp"
#include "core/object/object.hpp"


namespace Engine {
    namespace Geometry {
        class Primitive : public Object {
        public:
            Primitive() : Object() {
                mesh = std::make_unique<Ecs::Component::Mesh>();
            }
        };


        class Plane : public Primitive {
        public:
            Plane() : Primitive() {
                name = std::string("Plane");

                mesh->vertices = {
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
                mesh->indices = {
                    0, 1, 3,
                    1, 2, 3
                };
            }
        };


        class Cube : public Primitive {};
    }
}

#endif
