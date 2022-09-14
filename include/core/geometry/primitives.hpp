#ifndef __GEOMETRY_PRIMITIVE__
#define __GEOMETRY_PRIMITIVE__

#include <memory>

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
                   -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f,  0.5f,  0.0f,  0.0f,  0.0f, 1.0f,
                   -0.5f, -0.5f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f,  0.5f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.0f,  0.0f,  0.0f, 1.0f,
                   -0.5f, -0.5f,  0.0f,  0.0f,  0.0f, 1.0f
                };
                mesh->Initialize();
            }
        };


        class Cube : public Primitive {
        public:
            Cube() : Primitive() {
                name = std::string("Cube");

                mesh->vertices = {
                   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
                };
                mesh->Initialize();
            }
        };
    }
}

#endif
