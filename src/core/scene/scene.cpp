#include "core/scene/scene.hpp"

using namespace Engine::Scene;


Scene::Scene() {
    root = new RootEntity(std::string("root"));
}


Scene::~Scene() {
    delete root;
}


void Scene::Startup() {
    std::vector<Entity*> nodes;

    for (Entity *node : root->children) {
        nodes.push_back(node);
    }

    while (!nodes.empty()) {
        Entity *node = nodes.back();
        nodes.pop_back();

        for (Entity *child : node->children) {
            nodes.push_back(child);
        }

        node->Run();
    }
}


void Scene::Update() {
    std::vector<Entity*> nodes;

    for (Entity *node : root->children) {
        nodes.push_back(node);
    }

    while (!nodes.empty()) {
        Entity *node = nodes.back();
        nodes.pop_back();

        for (Entity *child : node->children) {
            nodes.push_back(child);
        }

        node->Draw();
    }
}
