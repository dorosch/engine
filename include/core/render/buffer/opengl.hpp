#ifndef __BUFFER_OPENGL__
#define __BUFFER_OPENGL__

#include <GL/glew.h>

#include "core/render/buffer/base.hpp"


namespace Engine {
    namespace Render {
        class OpenglVertexBuffer : public VertexBuffer {
        public:
            OpenglVertexBuffer();
            virtual ~OpenglVertexBuffer();
            void bind(float *, uint32_t);
            void unbind();
        };


        class OpenglIndexBuffer : public IndexBuffer {
        public:
            OpenglIndexBuffer();
            virtual ~OpenglIndexBuffer();
            void bind(uint32_t *, uint32_t);
            void unbind();
        };
    }
}

#endif
