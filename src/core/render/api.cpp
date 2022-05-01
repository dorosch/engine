#include "core/render/api.hpp"

using namespace Engine;


Render::Backend Render::GetBackendAPI() {
    return Render::BackendAPI;
}


void Render::SetBackendAPI(Render::Backend backendAPI) {
    Render::BackendAPI = backendAPI;
}
