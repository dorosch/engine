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

    // TODO: Change on GetInstance for ShaderProgram
    shader = std::make_unique<OpenglShaderProgram>();
    shader->Build(
        cwd / "resources" / "shaders" / "default.vert",
        cwd / "resources" / "shaders" / "default.frag"
    );

    lightingShader = std::make_unique<OpenglShaderProgram>();
    lightingShader->Build(
        cwd / "resources" / "shaders" / "graphics" / "lighting" / "lighting.vert",
        cwd / "resources" / "shaders" / "graphics" / "lighting" / "lighting.frag"
    );
}


void Render::Shutdown() {

}


void Render::RenderScene(Engine::Scene::Scene *scene, glm::mat4 MVP) {
    // TODO: Only for opengl backend

    std::shared_ptr<Graphics::Lighting::Light> lighting = scene->lighting[0];

    for (std::shared_ptr<Object> object : scene->root->entities) {
        RenderObject(object.get(), MVP, lighting.get());
    }

    // for (std::shared_ptr<Graphics::Lighting::Light> light : scene->lighting) {
    //     RenderObject(object.get(), MVP);
    // }

    // TODO: It's a peace of shit but necessary for a progress
    if (lighting->HasComponent(Ecs::Component::Type::LIGHT)) {
        lightingShader->Use();
        lightingShader->UniformPosition(
            "transform_position",
            lighting->transform->position[0],
            lighting->transform->position[1],
            lighting->transform->position[2]
        );
        lightingShader->UniformMatrix("MVP", MVP);

        lighting->mesh->VAO->bind();
        glDrawArrays(GL_TRIANGLES, 0, lighting->mesh->vertices.size() / 6);
        lighting->mesh->VAO->unbind();
    }
}


void Render::RenderObject(Object *object, glm::mat4 MVP, Graphics::Lighting::Light *lighting) {
    if (object->HasComponent(Ecs::Component::Type::MESH)) {
        // TODO: If object hasn't own shader component
        shader->Use();
        shader->UniformPosition(
            "transform_position",
            object->transform->position[0],
            object->transform->position[1],
            object->transform->position[2]
        );
        shader->UniformMatrix("MVP", MVP);
        shader->UniformPosition(
            "lighting",
            lighting->light->color[0],
            lighting->light->color[1],
            lighting->light->color[2]
        );
        shader->UniformPosition(
            "lightingPosition",
            lighting->transform->position[0],
            lighting->transform->position[1],
            lighting->transform->position[2]
        );

        object->mesh->VAO->bind();
        // TODO: Add calculation of the number of lines to the mesh method
        glDrawArrays(GL_TRIANGLES, 0, object->mesh->vertices.size() / 6);
        object->mesh->VAO->unbind();
    }
}
