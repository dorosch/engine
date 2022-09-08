#include "core/ecs/component.hpp"

#include <GL/glew.h>

using namespace Engine::Ecs::Component;


void Mesh::Initialize() {
    VAO = Engine::Render::VertexArray::GetInstance();
    VBO = Engine::Render::VertexBuffer::GetInstance();
    EBO = Engine::Render::IndexBuffer::GetInstance();

    VAO->bind();
    VBO->bind(vertices.data(), vertices.size() * sizeof(float));
    EBO->bind(indices.data(), indices.size() * sizeof(uint32_t));

    VAO->layout(3, 3 * sizeof(float), 0);

    VBO->unbind();
    VAO->unbind();
}
