#ifndef __SCENE_ENVIRONMENT__
#define __SCENE_ENVIRONMENT__

#include <memory>

#include "core/graphics/sky/skybox.hpp"

namespace Engine {
    namespace Scene {
        class Environment {
        public:
            std::unique_ptr<Graphics::Sky::Skybox> skybox;

            Environment();
            void startup();
        };
    }
}

#endif
