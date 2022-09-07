#ifndef __OBJECT__
#define __OBJECT__

#include "core/ecs/entity.hpp"


namespace Engine {
    class Object : public Ecs::Entity {
    public:
        Object() : Ecs::Entity() {};
    };
}

#endif
