#include "core/render/texture/opengl.hpp"

using namespace Engine::Render;


OpenglTexture::OpenglTexture() {
    glGenTextures(1, &this->object);
}


OpenglTexture::~OpenglTexture() {
    glDeleteTextures(1, &this->object);
}


void OpenglTexture::Build(std::filesystem::path path) {
    int width, height;
    glBindTexture(GL_TEXTURE_2D, this->object);

    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); 
}


void OpenglTexture::Bind() {
    glBindTexture(GL_TEXTURE_2D, this->object);
}
