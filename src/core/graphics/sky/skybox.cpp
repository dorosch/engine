#include "core/graphics/sky/skybox.hpp"

using namespace Engine::Graphics::Sky;


void Skybox::initialize() {
    std::filesystem::path cwd = std::filesystem::current_path();

    cubemap = Render::Cubemap::GetInstance();
    VAO = Render::VertexArray::GetInstance();
    VBO = Render::VertexBuffer::GetInstance();
    shader = Render::ShaderProgram::GetInstance();

    shader->Build(
        cwd / "resources" / "shaders" / "graphics" / "sky" / "skybox.vert",
        cwd / "resources" / "shaders" / "graphics" / "sky" / "skybox.frag"
    );

    std::vector<std::filesystem::path> faces = {
        cwd / "resources" / "textures" / "skybox" / "default" / "right.jpg",
        cwd / "resources" / "textures" / "skybox" / "default" / "left.jpg",
        cwd / "resources" / "textures" / "skybox" / "default" / "top.jpg",
        cwd / "resources" / "textures" / "skybox" / "default" / "bottom.jpg",
        cwd / "resources" / "textures" / "skybox" / "default" / "front.jpg",
        cwd / "resources" / "textures" / "skybox" / "default" / "back.jpg"
    };

    cubemap->Build(faces);

    // TODO: Reuse vertices from cube primitive
    float vertices[] = {
       -1000.0f,  1000.0f, -1000.0f,
       -1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f,  1000.0f, -1000.0f,
       -1000.0f,  1000.0f, -1000.0f,

       -1000.0f, -1000.0f,  1000.0f,
       -1000.0f, -1000.0f, -1000.0f,
       -1000.0f,  1000.0f, -1000.0f,
       -1000.0f,  1000.0f, -1000.0f,
       -1000.0f,  1000.0f,  1000.0f,
       -1000.0f, -1000.0f,  1000.0f,

        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,

       -1000.0f, -1000.0f,  1000.0f,
       -1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f, -1000.0f,  1000.0f,
       -1000.0f, -1000.0f,  1000.0f,

       -1000.0f,  1000.0f, -1000.0f,
        1000.0f,  1000.0f, -1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
       -1000.0f,  1000.0f,  1000.0f,
       -1000.0f,  1000.0f, -1000.0f,

       -1000.0f, -1000.0f, -1000.0f,
       -1000.0f, -1000.0f,  1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
       -1000.0f, -1000.0f,  1000.0f,
        1000.0f, -1000.0f,  1000.0f
    };

    VAO->bind();
    VBO->bind(vertices, sizeof(vertices));

    VAO->layout(3, 3 * sizeof(float), 0);

    VBO->unbind();
    VAO->unbind();
}


void Skybox::update(glm::mat4 MVP) {
    glDepthFunc(GL_LEQUAL);

    shader->Use();
    shader->UniformMatrix("MVP", MVP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->object);

    VAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    VAO->unbind();

    glDepthFunc(GL_LESS);
}
