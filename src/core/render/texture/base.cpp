#include "core/render/texture/base.hpp"
#include "core/render/texture/opengl.hpp"

using namespace Engine::Render;


Texture *Texture::GetInstance() {
    switch (Render::GetInstance()->GetBackendAPI()) {
        case Backend::OPENGL:
            return new OpenglTexture();
            break;
        default:
            throw std::logic_error("Texture - unsopported backend API");
    }
}
