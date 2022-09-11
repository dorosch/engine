#include "core/render/texture/base.hpp"
#include "core/render/texture/opengl.hpp"

using namespace Engine::Render;


Texture *Texture::GetInstance() {
    return new OpenglTexture();

    // TODO: Need to have static render backend
    // switch (Render::GetInstance()->GetBackendAPI()) {
    //     case Backend::OPENGL:
    //         return new OpenglTexture();
    //         break;
    //     default:
    //         throw std::logic_error("Texture - unsopported backend API");
    // }
}
