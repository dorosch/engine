#ifndef __OPENGL_TEXTURE__
#define __OPENGL_TEXTURE__

#include <SOIL/SOIL.h>

#include "core/render/texture/base.hpp"


namespace Engine {
    namespace Render {
        class OpenglTexture : public Texture {
        public:
            OpenglTexture();
            virtual ~OpenglTexture();
            void Build(std::filesystem::path);
            void Bind();
        };
    }
}

#endif
