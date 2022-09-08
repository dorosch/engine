#include "tools/debug/axes/opengl.hpp"

using namespace Tool::Debug;


OpenglDebugAxes::OpenglDebugAxes() {
    std::filesystem::path cwd = std::filesystem::current_path();

    this->shader = Engine::Render::ShaderProgram::GetInstance();
    this->shader->Build(
        cwd / "resources" / "shaders" / "debug" / "axes" / "shader.vert",
        cwd / "resources" / "shaders" / "debug" / "axes" / "shader.frag"
    );

    GLfloat vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &this->VAO);
    this->VBO = VertexBuffer::GetInstance();

    glBindVertexArray(this->VAO);
    this->VBO->bind(vertices, sizeof(vertices));

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
                
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    this->VBO->unbind();

    glBindVertexArray(0);
}


OpenglDebugAxes::~OpenglDebugAxes() {
    glDeleteVertexArrays(1, &this->VAO);

    delete this->shader;
}


void OpenglDebugAxes::SetMVP(glm::mat4 mvp) {
    this->MVP = mvp;
}


void OpenglDebugAxes::Enable() {
    this->shader->Use();
    this->shader->UniformMatrix("MVP", this->MVP);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
}
