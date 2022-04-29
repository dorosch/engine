#include "core/render/buffer/base.hpp"
#include "core/render/buffer/opengl.hpp"

using namespace Engine::Render;


VertexBuffer *VertexBuffer::GetInstance() {
    // TODO: Add switch for select target buffer implementation
    return new OpenglVertexBuffer();
}


IndexBuffer *IndexBuffer::GetInstance() {
    // TODO: Add switch for select target buffer implementation
    return new OpenglIndexBuffer();
}
