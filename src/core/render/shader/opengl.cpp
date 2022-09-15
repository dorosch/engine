#include "core/render/shader/opengl.hpp"

using namespace Engine::Render;


OpenglShaderProgram::OpenglShaderProgram() {
    this->object = glCreateProgram();
}


OpenglShaderProgram::~OpenglShaderProgram() {
    glDeleteProgram(this->object);
}


void OpenglShaderProgram::Build(
    std::filesystem::path vertexPath, std::filesystem::path fragmentPath
) {
    std::string vertexSource;
    std::string fragmentSource;

    try {
        vertexSource = this->ReadShaderFile(vertexPath);
    }
    catch (const std::exception& error) {
        logger->error(error.what());
    }

    try {
        fragmentSource = this->ReadShaderFile(fragmentPath);
    }
    catch (const std::exception& error) {
        logger->error(error.what());
    }

    std::optional<GLuint> vertex = this->Compile(
        Shader::VERTEX, vertexSource
    );
    std::optional<GLuint> fragment = this->Compile(
        Shader::FRAGMENT, fragmentSource
    );

    if (vertex) {
        glAttachShader(this->object, vertex.value());
    }
    else {
        this->logger->warning(std::string("Shader compilation result not received"));
    }

    if (fragment) {
        glAttachShader(this->object, fragment.value());
    }
    else {
        this->logger->warning(std::string("Shader compilation result not received"));
    }

    this->Linking();

    if (vertex) {
        glDeleteShader(vertex.value());
    }

    if (fragment) {
        glDeleteShader(fragment.value());
    }
}


std::optional<uint32_t> OpenglShaderProgram::Compile(Shader type, std::string content) {
    GLint compileStatus;
    uint32_t shader;
    const char *shaderContent = content.c_str();

    switch (type) {
        case Shader::VERTEX:
            shader = glCreateShader(GL_VERTEX_SHADER);
            break;
        case Shader::FRAGMENT:
            shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            throw std::logic_error("Shader type is not implement");
    }

    glShaderSource(shader, 1, &shaderContent, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus) {
        return shader;
    }
    else {
        GLchar errorMessage[512];

        glGetShaderInfoLog(shader, 512, NULL, errorMessage);

        this->logger->error(
            fmt::format("Shader compile error {}", errorMessage)
        );

        return std::nullopt;
    }
}


void OpenglShaderProgram::Linking() {
    GLint linkingStatus;

    glLinkProgram(this->object);
    glGetProgramiv(this->object, GL_LINK_STATUS, &linkingStatus);

    if (!linkingStatus) {
        GLchar errorMessage[512];

        glGetProgramInfoLog(this->object, 512, NULL, errorMessage);

        this->logger->error(
            fmt::format("Shader program linking error {}", errorMessage)
        );
    }
}


void OpenglShaderProgram::Use() {
    glUseProgram(this->object);
}


void OpenglShaderProgram::UniformFloat(const char *attribute, float value) {
    GLint variable = glGetUniformLocation(this->object, attribute);
    glUniform1f(variable, value);
}


void OpenglShaderProgram::UniformInt(const char *attribute, int value) {
    GLint variable = glGetUniformLocation(this->object, attribute);
    glUniform1i(variable, value);
}


void OpenglShaderProgram::UniformVector(const char *attribute, glm::vec3 vector){
    GLint variable = glGetUniformLocation(this->object, attribute);
    glUniform3fv(variable, 1, glm::value_ptr(vector));
}


void OpenglShaderProgram::UniformMatrix(const char *attribure, glm::mat4 matrix) {
    GLuint targetAttribute = glGetUniformLocation(this->object, attribure);
    glUniformMatrix4fv(targetAttribute, 1, GL_FALSE, glm::value_ptr(matrix));
}
