#include "core/render/buffer/base.hpp"
#include "core/render/buffer/opengl.hpp"

using namespace Engine::Render;


std::unique_ptr<VertexBuffer> VertexBuffer::GetInstance() {
    return std::make_unique<OpenglVertexBuffer>();

    // switch (Render::GetInstance()->GetBackendAPI()) {
    //     case Backend::OPENGL:
    //         return std::make_unique<OpenglVertexBuffer>();
    //     default:
    //         throw std::logic_error("VertexBuffer - unsopported backend API");
    // }
}


std::unique_ptr<IndexBuffer> IndexBuffer::GetInstance() {
    return std::make_unique<OpenglIndexBuffer>();

    // switch (Render::GetInstance()->GetBackendAPI()) {
    //     case Backend::OPENGL:
    //         return std::make_unique<OpenglIndexBuffer>();
    //     default:
    //         throw std::logic_error("IndexBuffer - unsopported backend API");
    // }
}


std::unique_ptr<VertexArray> VertexArray::GetInstance() {
    return std::make_unique<OpenglVertexArray>();

    // switch (Render::GetInstance()->GetBackendAPI()) {
    //     case Backend::OPENGL:
    //         return std::make_unique<OpenglVertexArray>();
    //     default:
    //         throw std::logic_error("VertexArray - unsopported backend API");
    // }
}
