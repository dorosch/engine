#include "tools/debug/grid/base.hpp"
#include "tools/debug/grid/opengl.hpp"

using namespace Tool::Debug;


DebugFloorGrid *DebugFloorGrid::GetInstance() {
    // TODO: Add switch to select render backend
    return new OpenglDebugFloorGrid();
}
