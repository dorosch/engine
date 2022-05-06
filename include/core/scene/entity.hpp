#ifndef __ENTITY__
#define __ENTITY__

#include <memory>
#include <vector>
#include <string>


namespace Engine {
    namespace Scene {
        static uint32_t nextEntityId = 0;


        class Entity {
        public:
            uint32_t id;
            std::string name;
            Entity *parent = nullptr;
            std::vector<Entity*> children;

            Entity(std::string);
            ~Entity();
            virtual void Run() = 0;
            virtual void Draw() = 0;
        };
    }
}

#endif
