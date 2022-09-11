#ifndef __GRAPHICS_SKY_SKYBOX__
#define __GRAPHICS_SKY_SKYBOX__

#include <filesystem>

#include <glm/glm.hpp>

#include "core/render/buffer/base.hpp"
#include "core/render/shader/base.hpp"
#include "core/render/texture/cubemap/base.hpp"

namespace Engine {
    namespace Graphics {
        namespace Sky {
            class Skybox {
            private:
                Render::Cubemap *cubemap;
                Render::ShaderProgram *shader;
                std::unique_ptr<Render::VertexArray> VAO;
                std::unique_ptr<Render::VertexBuffer> VBO;

            public:
                void initialize();
                void update(glm::mat4);
            };
        }
    }
}

#endif
