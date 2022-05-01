#include "core/render/api.hpp"
#include "tools/debug/grid/base.hpp"
#include "tools/debug/grid/opengl.hpp"

using namespace Tool::Debug;


DebugFloorGrid *DebugFloorGrid::GetInstance() {
    switch (Engine::Render::GetBackendAPI()) {
        case Engine::Render::Backend::OPENGL:
            return new OpenglDebugFloorGrid();
            break;
        default:
            throw std::logic_error("OpenglDebugFloorGrid - unsopported backend API");
    }
}
