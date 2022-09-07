#include "core/render/api.hpp"

using namespace Engine::Render;


std::unique_ptr<Render> Render::singleton = nullptr;


std::unique_ptr<Render>& Render::GetInstance() {
    if(singleton == nullptr) {
        singleton = std::make_unique<Render>();
    }

    return singleton;
}


Backend Render::GetBackendAPI() {
    return BackendAPI;
}


void Render::SetBackendAPI(Backend backendAPI) {
    BackendAPI = backendAPI;
}


void Render::RenderScene(Engine::Scene::Scene *scene) {

}
