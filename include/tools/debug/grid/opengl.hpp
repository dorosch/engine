#ifndef __DEBUG_GRID_OPENGL__
#define __DEBUG_GRID_OPENGL__

#include <memory>
#include <vector>

#include <GL/glew.h>

#include "tools/debug/grid/base.hpp"
#include "core/render/buffer/base.hpp"
#include "core/render/shader/base.hpp"


namespace Tool {
    namespace Debug {
        using namespace Engine::Render;


        class OpenglDebugFloorGrid : public DebugFloorGrid {
        protected:
            GLuint VAO;
            glm::mat4 MVP;
            std::vector<glm::vec3> vertices;
            std::unique_ptr<VertexBuffer> VBO;
            ShaderProgram *shader = nullptr;

        public:
            OpenglDebugFloorGrid();
            virtual ~OpenglDebugFloorGrid();
            // TODO: Rename MVP matrix
            void SetMVP(glm::mat4);
            void Enable();
        };
    }
}

#endif
