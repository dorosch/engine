#ifndef __SHADER_OPENGL__
#define __SHADER_OPENGL__

#include <GL/glew.h>
#include <fmt/format.h>

#include "core/render/shader/base.hpp"


namespace Engine {
    namespace Render {
        class OpenglShaderProgram : public ShaderProgram {
        private:
            GLuint object;

        public:
            OpenglShaderProgram();
            ~OpenglShaderProgram();

            std::optional<uint32_t> Compile(Shader, std::string);
            void Linking();

            void Build(std::filesystem::path, std::filesystem::path);

            void Use();

            // TODO: Change to Uniform3f, Uniform4m
            void UniformColor(const char *, float, float, float);
            void UniformPosition(const char *, float, float, float);
            void UniformMatrix(const char *, glm::mat4);
        };
    }
}

#endif
