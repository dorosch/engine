#include "core/ecs/component.hpp"

#include <GL/glew.h>

using namespace Engine::Ecs::Component;


Mesh::Mesh() {
    VAO = Engine::Render::VertexArray::GetInstance();
    VBO = Engine::Render::VertexBuffer::GetInstance();
    EBO = Engine::Render::IndexBuffer::GetInstance();

    GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left 
    };
    uint32_t indices[] = {
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    // glBindVertexArray(VAO->object);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO->object);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO->object);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glBindVertexArray(0);

    VAO->bind();
    VBO->bind(vertices, sizeof(vertices));
    EBO->bind(indices, sizeof(indices));

    VAO->layout(3, 3 * sizeof(float), 0);

    VBO->unbind();
    VAO->unbind();
}
