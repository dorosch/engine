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

    // TODO: Add logger warning if more lights are added than the shader supports
    for (std::shared_ptr<Object> object : scene->root->entities) {
        RenderObject(object.get(), projection, view, cameraPosition, scene->lighting);
    }

    for (std::shared_ptr<Graphics::Lighting::Light> light : scene->lighting) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, light->transform->position);
        model = glm::rotate(model, glm::radians(180.0f), light->transform->rotation);
        model = glm::scale(model, light->transform->scale);

        lightingShader->Use();
        lightingShader->UniformMatrix("model", model);
        lightingShader->UniformMatrix("view", view);
        lightingShader->UniformMatrix("projection", projection);

        // If the editor is enabled, then draw where the light source is
        light->mesh->VAO->bind();
        glDrawArrays(GL_TRIANGLES, 0, light->mesh->vertices.size() / 6);
        light->mesh->VAO->unbind();
    }

    scene->environment->skybox->update(projection * view);
}


void Render::RenderObject(
        Object *object, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition,
        std::vector<std::shared_ptr<Graphics::Lighting::Light>> &lights
) {
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

        shader->UniformInt("lightSourcesCount", lights.size());

        for (size_t index = 0; index < lights.size(); index++) {
            std::shared_ptr<Graphics::Lighting::Light> light = lights[index];

            shader->UniformVector(fmt::format("lights[{}].position", index), light->transform->position);
            shader->UniformVector(fmt::format("lights[{}].color", index), light->light->color);
            shader->UniformVector(fmt::format("lights[{}].ambient", index), light->light->ambient);
            shader->UniformVector(fmt::format("lights[{}].diffuse", index), light->light->diffuse);
            shader->UniformVector(fmt::format("lights[{}].specular", index), light->light->specular);
            shader->UniformFloat(fmt::format("lights[{}].intensity", index), light->light->intensity);

            if (light->light->lightType == Graphics::Lighting::Type::DIRECTIONAL) {
                shader->UniformInt(fmt::format("lights[{}].type", index), Graphics::Lighting::Type::DIRECTIONAL);
                shader->UniformVector(fmt::format("lights[{}].direction", index), light->light->direction);
            }
            else if (light->light->lightType == Graphics::Lighting::Type::POINT) {
                shader->UniformInt(fmt::format("lights[{}].type", index), Graphics::Lighting::Type::POINT);
                shader->UniformFloat(fmt::format("lights[{}].constant", index), light->light->constant);
                shader->UniformFloat(fmt::format("lights[{}].linear", index), light->light->linear);
                shader->UniformFloat(fmt::format("lights[{}].quadratic", index), light->light->quadratic);
            }
            else if (light->light->lightType == Graphics::Lighting::Type::SPOT) {
                shader->UniformInt(fmt::format("lights[{}].type", index), Graphics::Lighting::Type::SPOT);
                shader->UniformVector(fmt::format("lights[{}].direction", index), light->light->direction);
                shader->UniformFloat(fmt::format("lights[{}].constant", index), light->light->constant);
                shader->UniformFloat(fmt::format("lights[{}].linear", index), light->light->linear);
                shader->UniformFloat(fmt::format("lights[{}].quadratic", index), light->light->quadratic);
                shader->UniformFloat(fmt::format("lights[{}].cutOff", index), light->light->cutOff);
                shader->UniformFloat(fmt::format("lights[{}].outerCutOff", index), light->light->outerCutOff);
            }
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
