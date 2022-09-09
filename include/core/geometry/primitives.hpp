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
                    0.5f,  0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,
                   -0.5f, -0.5f, 0.0f,
                   -0.5f,  0.5f, 0.0f
                };
                mesh->indices = {
                    0, 1, 3,
                    1, 2, 3
                };
                mesh->Initialize();
            }
        };


        class Cube : public Primitive {
        public:
            Cube() : Primitive() {
                name = std::string("Cube");

                mesh->vertices = {
                   -1.0, -1.0,  1.0,
                    1.0, -1.0,  1.0,
                    1.0,  1.0,  1.0,
                   -1.0,  1.0,  1.0,
                   -1.0, -1.0, -1.0,
                    1.0, -1.0, -1.0,
                    1.0,  1.0, -1.0,
                   -1.0,  1.0, -1.0
                };
                mesh->indices = {
                    0, 1, 2,
		            2, 3, 0,
		            1, 5, 6,
		            6, 2, 1,
		            7, 6, 5,
		            5, 4, 7,
		            4, 0, 3,
		            3, 7, 4,
                    4, 5, 1,
		            1, 0, 4,
		            3, 2, 6,
		            6, 7, 3
                };
                mesh->Initialize();
            }
        };
    }
}

#endif
