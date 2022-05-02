#include "core/scene/entity.hpp"

using namespace Engine::Scene;


Entity::Entity(std::string name) {
    this->name = name;
    id = nextEntityId;
    nextEntityId = nextEntityId + 1;
}


Entity::~Entity() {
    if (nextEntityId > 0) {
        nextEntityId = nextEntityId - 1;
    }
}
