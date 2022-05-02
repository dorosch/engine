#include "core/scene/scene.hpp"

using namespace Engine::Scene;


Scene::Scene() {
    root = new Entity(std::string("root"));
}


Scene::~Scene() {
    delete root;
}
