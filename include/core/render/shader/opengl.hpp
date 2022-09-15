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
            void Use();
            void Linking();
            void Build(std::filesystem::path, std::filesystem::path);
            void UniformInt(const char *, int);
            void UniformFloat(const char *, float);
            void UniformVector(const char *, glm::vec3);
            void UniformMatrix(const char *, glm::mat4);
        };
    }
}

#endif
