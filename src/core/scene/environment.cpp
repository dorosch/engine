#include "core/scene/environment.hpp"

using namespace Engine::Scene;


Environment::Environment() {
    skybox = std::make_unique<Graphics::Sky::Skybox>();
}


void Environment::startup() {
    skybox->initialize();
}
