#include "core/ecs/component.hpp"

using namespace Engine::Ecs::Component;


Mesh::Mesh() {
    VAO = Engine::Render::VertexArray::GetInstance();
    VBO = Engine::Render::VertexBuffer::GetInstance();
    EBO = Engine::Render::IndexBuffer::GetInstance();
}
