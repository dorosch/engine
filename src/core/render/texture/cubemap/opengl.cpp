#include "core/render/texture/cubemap/opengl.hpp"

using namespace Engine::Render;


OpenglCubemap::OpenglCubemap() {
    glGenTextures(1, &this->object);
}


OpenglCubemap::~OpenglCubemap() {
    glDeleteTextures(1, &this->object);
}


void OpenglCubemap::Build(std::vector<std::filesystem::path> textures) {
    int width, height;
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->object);

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::filesystem::path texture = textures[i];

        unsigned char *image = SOIL_load_image(
            texture.c_str(), &width, &height, 0, SOIL_LOAD_RGB
        );

        if (image) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
            );
        }
        else {
            logger->error(fmt::format("Cubemap texture failed to load: {}", texture.c_str()));
        }

        SOIL_free_image_data(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void OpenglCubemap::Bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->object);
}
