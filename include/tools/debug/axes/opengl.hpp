#ifndef __DEBUG_AXES_OPENGL__
#define __DEBUG_AXES_OPENGL__

#include <GL/glew.h>

#include "tools/debug/axes/base.hpp"
#include "core/render/buffer/base.hpp"
#include "core/render/shader/base.hpp"


namespace Tool {
    namespace Debug {
        using namespace Engine::Render;


        class OpenglDebugAxes : public DebugAxes {
        protected:
            GLuint VAO;
            glm::mat4 MVP;
            VertexBuffer *VBO = nullptr;
            ShaderProgram *shader = nullptr;

        public:
            OpenglDebugAxes();
            virtual ~OpenglDebugAxes();
            // TODO: Rename MVP matrix
            void SetMVP(glm::mat4);
            void Enable();
        };
    }
}

#endif
