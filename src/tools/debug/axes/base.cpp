#include "core/render/api.hpp"
#include "tools/debug/axes/base.hpp"
#include "tools/debug/axes/opengl.hpp"

using namespace Tool::Debug;


DebugAxes *DebugAxes::GetInstance() {
    switch (Engine::Render::Render::GetInstance()->GetBackendAPI()) {
        case Engine::Render::Backend::OPENGL:
            return new OpenglDebugAxes();
            break;
        default:
            throw std::logic_error("DebugAxes - unsopported backend API");
    }
}
