#include "core/render/api.hpp"
#include "core/render/shader/base.hpp"
#include "core/render/shader/opengl.hpp"

using namespace Engine::Render;


std::string ShaderProgram::ReadShaderFile(std::filesystem::path path) {    
    std::ifstream file(path);
    
    if (file.is_open()) {
        std::string content(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
        file.close();

        return content;
    }

    throw std::runtime_error(
        fmt::format("Cann't open file: {}", path.c_str())
    );
}


ShaderProgram *ShaderProgram::GetInstance() {
    switch (Render::GetInstance()->GetBackendAPI()) {
        case Backend::OPENGL:
            return new OpenglShaderProgram();
            break;
        default:
            throw std::logic_error("ShaderProgram - unsopported backend API");
    }
}
