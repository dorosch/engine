#ifndef __CUBEMAP_OPENGL__
#define __CUBEMAP_OPENGL__

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "core/render/texture/cubemap/base.hpp"


namespace Engine {
    namespace Render {
        class OpenglCubemap : public Cubemap {
        public:
            OpenglCubemap();
            virtual ~OpenglCubemap();
            void Build(std::vector<std::filesystem::path>);
            void Bind();
        };
    }
}

#endif
