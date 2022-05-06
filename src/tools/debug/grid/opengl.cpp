#include "tools/debug/grid/opengl.hpp"

using namespace Tool::Debug;


OpenglDebugFloorGrid::OpenglDebugFloorGrid() {
    std::filesystem::path cwd = std::filesystem::current_path();

    this->shader = Engine::Render::ShaderProgram::GetInstance();
    this->shader->Build(
        cwd / "resources" / "shaders" / "debug" / "floor-grid" / "shader.vert",
        cwd / "resources" / "shaders" / "debug" / "floor-grid" / "shader.frag"
    );

    for (uint32_t x = 0; x <= 50; ++x) {
        vertices.push_back(glm::vec3(x - 25.0f, 0.0f,  25.0f));
        vertices.push_back(glm::vec3(x - 25.0f, 0.0f, -25.0f));
    }

    for (uint32_t z = 0; z <= 50; ++z) {
        vertices.push_back(glm::vec3( 25.0f, 0.0f, z - 25.0f));
        vertices.push_back(glm::vec3(-25.0f, 0.0f, z - 25.0f));
    }

    glGenVertexArrays(1, &this->VAO);
    this->VBO = VertexBuffer::GetInstance();

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO->object);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    this->VBO->unbind();

    glBindVertexArray(0);
}


OpenglDebugFloorGrid::~OpenglDebugFloorGrid() {
    glDeleteVertexArrays(1, &this->VAO);

    delete this->VBO;
    delete this->shader;
}


void OpenglDebugFloorGrid::SetMVP(glm::mat4 mvp) {
    this->MVP = mvp;
}


void OpenglDebugFloorGrid::Enable() {
    this->shader->Use();
    this->shader->UniformMatrix("MVP", this->MVP);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINES, 0, this->vertices.size());
    glBindVertexArray(0);
}
