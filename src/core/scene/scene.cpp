#include "core/scene/scene.hpp"

using namespace Engine::Scene;


Scene::Scene() {
    root = std::make_unique<Node>(std::string("root"));
}


void Scene::Startup() {
    // std::vector<std::unique_ptr<Node>> nodes;

    // for (std::unique_ptr<Node> node : root->children) {
    //     nodes.push_back(node);
    // }

    // while (!nodes.empty()) {
    //     std::unique_ptr<Node> node = nodes.back();
    //     nodes.pop_back();

    //     for (std::unique_ptr<Node> child : node->children) {
    //         nodes.push_back(child);
    //     }

    //     // node->Run();
    // }
}


void Scene::Update() {
    // std::vector<std::unique_ptr<Node>> nodes;

    // for (std::unique_ptr<Node> node : root->children) {
    //     nodes.push_back(node);
    // }

    // while (!nodes.empty()) {
    //     std::unique_ptr<Node> node = nodes.back();
    //     nodes.pop_back();

    //     for (std::unique_ptr<Node> child : node->children) {
    //         nodes.push_back(child);
    //     }

    //     // node->Draw();
    // }
}
