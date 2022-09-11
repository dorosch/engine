#include "core/scene/scene.hpp"

using namespace Engine::Scene;


Scene::Scene() {
    root = std::make_shared<Node>(std::string("root"));
    environment = std::make_unique<Environment>();
}


void Scene::startup() {
    environment->startup();
}
