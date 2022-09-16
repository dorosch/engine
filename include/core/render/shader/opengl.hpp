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
            void UniformInt(const std::string &, int);
            void UniformFloat(const std::string &, float);
            void UniformVector(const std::string &, glm::vec3);
            void UniformMatrix(const std::string &, glm::mat4);
        };
    }
}

#endif
