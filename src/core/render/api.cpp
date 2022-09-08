#include "core/render/api.hpp"

#include <iostream>

using namespace Engine::Render;


std::shared_ptr<Render> Render::singleton = nullptr;


std::shared_ptr<Render>& Render::GetInstance() {
    if(singleton == nullptr) {
        singleton = std::make_shared<Render>();
    }

    return singleton;
}


Backend Render::GetBackendAPI() {
    return BackendAPI;
}


void Render::SetBackendAPI(Backend backendAPI) {
    BackendAPI = backendAPI;
}


void Render::Startup() {
    std::filesystem::path cwd = std::filesystem::current_path();

    shader = std::make_unique<OpenglShaderProgram>();
    shader->Build(
        cwd / "resources" / "shaders" / "default.vert",
        cwd / "resources" / "shaders" / "default.frag"
    );
}


void Render::Shutdown() {

}


void Render::RenderScene(Engine::Scene::Scene *scene) {
    // TODO: Only for opengl backend

    for (std::shared_ptr<Object> object : scene->root->entities) {
        if (object->HasComponent(Ecs::Component::Type::MESH)) {
            // TODO: If object hasn't own shader component
            shader->Use();

            // object->mesh->VAO->bind();
             glBindVertexArray(object->mesh->VAO->object);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // object->mesh->VAO->unbind();
            glBindVertexArray(0);
        }
    }
}
