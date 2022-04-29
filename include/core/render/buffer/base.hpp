#ifndef __BUFFER_BASE__
#define __BUFFER_BASE__

#include <memory>


namespace Engine {
    namespace Render {
        class Buffer {
        public:
            uint32_t object;
        };


        class VertexBuffer : public Buffer {
        public:
            static VertexBuffer *GetInstance();
            virtual void bind(float *, uint32_t) = 0;
            virtual void unbind() = 0;
        };


        class IndexBuffer : public Buffer {
        public:
            uint32_t count = 0;

            static IndexBuffer *GetInstance();
            virtual void bind(uint32_t *, uint32_t) = 0;
            virtual void unbind() = 0;
        };
    }
}

#endif
