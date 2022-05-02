#ifndef __SCENE__
#define __SCENE__

#include "core/scene/entity.hpp"


namespace Engine {
    namespace Scene {
        class Scene {
        public:
            Entity *root = nullptr;

            Scene();
            ~Scene();
        };
    }
}

#endif
