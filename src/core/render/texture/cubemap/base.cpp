#include "core/render/texture/cubemap/base.hpp"
#include "core/render/texture/cubemap/opengl.hpp"

using namespace Engine::Render;


Cubemap *Cubemap::GetInstance() {
    switch (Render::GetInstance()->GetBackendAPI()) {
        case Backend::OPENGL:
            return new OpenglCubemap();
            break;
        default:
            throw std::logic_error("Cubemap - unsopported backend API");
    }
}
