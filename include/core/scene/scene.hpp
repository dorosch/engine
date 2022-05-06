#ifndef __SCENE__
#define __SCENE__

#include "core/scene/entity.hpp"


namespace Engine {
    namespace Scene {
        class RootEntity : public Entity {
        public:
            RootEntity(std::string name) : Entity(name) {}
            void Run() {}
            void Draw() {}
        };


        class Scene {
        public:
            RootEntity *root = nullptr;

            Scene();
            ~Scene();
            void Startup();
            void Update();
        };
    }
}

#endif
