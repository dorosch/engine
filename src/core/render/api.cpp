#include "core/render/api.hpp"

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
    defaultShader = std::make_unique<OpenglShaderProgram>();
    defaultShader->Build(
        cwd / "resources" / "shaders" / "default.vert",
        cwd / "resources" / "shaders" / "default.frag"
    );

    // TODO: Apply for light source
    lightingShader = std::make_unique<OpenglShaderProgram>();
    lightingShader->Build(
        cwd / "resources" / "shaders" / "graphics" / "lighting" / "lighting.vert",
        cwd / "resources" / "shaders" / "graphics" / "lighting" / "lighting.frag"
    );

    materialShader = std::make_unique<OpenglShaderProgram>();
    materialShader->Build(
        cwd / "resources" / "shaders" / "default.vert",
        cwd / "resources" / "shaders" / "material.frag"
    );
}


void Render::Shutdown() {

}


void Render::RenderScene(Engine::Scene::Scene *scene, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition) {
    // TODO: Only for opengl backend

    std::shared_ptr<Graphics::Lighting::Light> lighting = scene->lighting[0];

    for (std::shared_ptr<Object> object : scene->root->entities) {
        RenderObject(object.get(), projection, view, cameraPosition, lighting.get());
    }

    if (lighting->HasComponent(Ecs::Component::Type::LIGHT)) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lighting->transform->position);
        model = glm::rotate(model, glm::radians(180.0f), lighting->transform->rotation);
        model = glm::scale(model, lighting->transform->scale);

        lightingShader->Use();
        lightingShader->UniformMatrix("model", model);
        lightingShader->UniformMatrix("view", view);
        lightingShader->UniformMatrix("projection", projection);

        // If the editor is enabled, then draw where the light source is
        lighting->mesh->VAO->bind();
        glDrawArrays(GL_TRIANGLES, 0, lighting->mesh->vertices.size() / 6);
        lighting->mesh->VAO->unbind();
    }

    scene->environment->skybox->update(projection * view);
}


void Render::RenderObject(Object *object, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, Graphics::Lighting::Light *lighting) {
    if (object->HasComponent(Ecs::Component::Type::MESH)) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, object->transform->position);
        model = glm::rotate(model, glm::radians(180.0f), object->transform->rotation);
        model = glm::scale(model, object->transform->scale);

        if (object->HasComponent(Ecs::Component::Type::MATERIAL)) {
            shader = materialShader.get();
        }
        else {
            shader = defaultShader.get();
        }

        shader->Use();

        shader->UniformMatrix("model", model);
        shader->UniformMatrix("view", view);
        shader->UniformMatrix("projection", projection);

        shader->UniformVector("viewPosition", cameraPosition);

        shader->UniformVector("light.position", lighting->transform->position);
        shader->UniformVector("light.color", lighting->light->color);
        shader->UniformVector("light.ambient", lighting->light->ambient);
        shader->UniformVector("light.diffuse", lighting->light->diffuse);
        shader->UniformVector("light.specular", lighting->light->specular);
        shader->UniformFloat("light.intensity", lighting->light->intensity);

        if (lighting->light->lightType == Graphics::Lighting::Type::DIRECTIONAL) {
            shader->UniformInt("light.type", Graphics::Lighting::Type::DIRECTIONAL);
            shader->UniformVector("light.direction", lighting->light->direction);
        }
        else if (lighting->light->lightType == Graphics::Lighting::Type::POINT) {
            shader->UniformInt("light.type", Graphics::Lighting::Type::POINT);
            shader->UniformFloat("light.constant", lighting->light->constant);
            shader->UniformFloat("light.linear", lighting->light->linear);
            shader->UniformFloat("light.quadratic", lighting->light->quadratic);
        }
        else if (lighting->light->lightType == Graphics::Lighting::Type::SPOT) {
            shader->UniformInt("light.type", Graphics::Lighting::Type::SPOT);
            shader->UniformVector("light.direction", lighting->light->direction);
            shader->UniformFloat("light.constant", lighting->light->constant);
            shader->UniformFloat("light.linear", lighting->light->linear);
            shader->UniformFloat("light.quadratic", lighting->light->quadratic);
            shader->UniformFloat("light.cutOff", glm::cos(glm::radians(lighting->light->cutOff)));
            shader->UniformFloat("light.outerCutOff", glm::cos(glm::radians(lighting->light->outerCutOff)));
        }

        if (object->HasComponent(Ecs::Component::Type::MATERIAL)) {
            shader->UniformVector("material.color", object->material->color);
            shader->UniformVector("material.ambient", object->material->ambient);
            shader->UniformVector("material.diffuse", object->material->diffuse);
            shader->UniformVector("material.specular", object->material->specular);
            shader->UniformFloat("material.shininess", object->material->shininess);
        }

        object->mesh->VAO->bind();
        // TODO: Add calculation of the number of lines to the mesh method
        glDrawArrays(GL_TRIANGLES, 0, object->mesh->vertices.size() / 6);
        object->mesh->VAO->unbind();
    }
}
