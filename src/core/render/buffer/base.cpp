#include "core/render/api.hpp"
#include "core/render/buffer/base.hpp"
#include "core/render/buffer/opengl.hpp"

using namespace Engine::Render;


VertexBuffer *VertexBuffer::GetInstance() {
    switch (Render::GetInstance()->GetBackendAPI()) {
        case Backend::OPENGL:
            return new OpenglVertexBuffer();
            break;
        default:
            throw std::logic_error("VertexBuffer - unsopported backend API");
    }
}


IndexBuffer *IndexBuffer::GetInstance() {
    switch (Render::GetInstance()->GetBackendAPI()) {
        case Backend::OPENGL:
            return new OpenglIndexBuffer();
            break;
        default:
            throw std::logic_error("IndexBuffer - unsopported backend API");
    }
}


VertexArray *VertexArray::GetInstance() {
    switch (Render::GetInstance()->GetBackendAPI()) {
        case Backend::OPENGL:
            return new OpenglVertexArray();
            break;
        default:
            throw std::logic_error("VertexArray - unsopported backend API");
    }
}
