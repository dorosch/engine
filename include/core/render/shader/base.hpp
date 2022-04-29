#ifndef __SHADER_BASE__
#define __SHADER_BASE__

#include <string>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <iostream>
#include <optional>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tools/logger.hpp"


namespace Engine {
    namespace Render {
        enum Shader {
            VERTEX,
            FRAGMENT
        };


        class ShaderProgram {
        protected:
            std::unique_ptr<Tool::Logger::Logger> logger = 
                std::make_unique<Tool::Logger::Logger>("shader");

            std::string ReadShaderFile(std::filesystem::path);

        public:
            static ShaderProgram *GetInstance();
            virtual void Build(std::filesystem::path, std::filesystem::path) = 0;

            virtual std::optional<uint32_t> Compile(Shader, std::string) = 0;
            virtual void Linking() = 0;
            virtual void Use() = 0;

            // TODO: Change to Uniform3f, Uniform4m
            virtual void UniformColor(const char *, float, float, float) = 0;
            virtual void UniformPosition(const char *, float, float, float) = 0;
            virtual void UniformMatrix(const char *, glm::mat4) = 0;
        };
    }
}

#endif
