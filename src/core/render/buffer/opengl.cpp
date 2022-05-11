#include "core/render/buffer/opengl.hpp"

using namespace Engine::Render;


OpenglVertexBuffer::OpenglVertexBuffer() {
    glGenBuffers(1, &this->object);
}


OpenglVertexBuffer::~OpenglVertexBuffer() {
    glDeleteBuffers(1, &this->object);
}


void OpenglVertexBuffer::bind(float *vertices, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, object);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}


void OpenglVertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


OpenglIndexBuffer::OpenglIndexBuffer() {
    glGenBuffers(1, &this->object);
}


OpenglIndexBuffer::~OpenglIndexBuffer() {
    glDeleteBuffers(1, &this->object);
}


void OpenglIndexBuffer::bind(uint32_t *indices, uint32_t count) {
    this->count = count;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}


void OpenglIndexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


OpenglVertexArray::OpenglVertexArray() {
    glGenVertexArrays(1, &this->object);
}


OpenglVertexArray::~OpenglVertexArray() {
    glDeleteVertexArrays(1, &this->object);
}


void OpenglVertexArray::bind() {
    glBindVertexArray(this->object);
}


void OpenglVertexArray::unbind() {
    glBindVertexArray(0);
}


void OpenglVertexArray::layout(uint32_t length, uint32_t size, uint32_t offset) {
    glVertexAttribPointer(this->layouts, length, GL_FLOAT, GL_FALSE, size, (GLvoid*)offset);
    glEnableVertexAttribArray(this->layouts);

    this->layouts += 1;
}
