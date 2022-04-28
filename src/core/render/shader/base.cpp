#include "core/render/shader/base.hpp"
#include "core/render/shader/opengl.hpp"

using namespace Engine::Render;


std::string ShaderProgram::ReadShaderFile(std::filesystem::path path) {
    if (!std::filesystem::exists(path)) {
        // TODO: Change on logger
        std::cout << "Error: file not exists" << std::endl;
    }

    std::ifstream file(path);
    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    return content;
}


ShaderProgram *ShaderProgram::GetInstance() {
    // TODO: Add switch for select target shader implementation
    return new OpenglShaderProgram();
}
