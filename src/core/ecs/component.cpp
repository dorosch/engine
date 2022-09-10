#include "core/ecs/component.hpp"

#include <GL/glew.h>

using namespace Engine::Ecs::Component;


void Mesh::Initialize() {
    VAO = Engine::Render::VertexArray::GetInstance();
    VBO = Engine::Render::VertexBuffer::GetInstance();

    VAO->bind();
    VBO->bind(vertices.data(), vertices.size() * sizeof(float));

    VAO->layout(3, 6 * sizeof(float), 0);
    VAO->layout(3, 6 * sizeof(float), 3 * sizeof(float));

    VBO->unbind();
    VAO->unbind();
}
