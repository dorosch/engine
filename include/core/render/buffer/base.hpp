#ifndef __BUFFER_BASE__
#define __BUFFER_BASE__

#include <memory>


namespace Engine {
    namespace Render {
        class Buffer {
        public:
            uint32_t object = 0;
        };


        class VertexBuffer : public Buffer {
        public:
            static std::unique_ptr<VertexBuffer> GetInstance();
            virtual void bind(float *, uint32_t) = 0;
            virtual void unbind() = 0;
        };


        class IndexBuffer : public Buffer {
        public:
            uint32_t count = 0;

            static std::unique_ptr<IndexBuffer> GetInstance();
            virtual void bind(uint32_t *, uint32_t) = 0;
            virtual void unbind() = 0;
        };


        class VertexArray : public Buffer {
        public:
            uint32_t layouts = 0;

            static std::unique_ptr<VertexArray> GetInstance();
            virtual void bind() = 0;
            virtual void unbind() = 0;
            virtual void layout(uint32_t, uint32_t, uint32_t) = 0;
        };
    }
}

#endif
