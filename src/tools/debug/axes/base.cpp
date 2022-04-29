#include "tools/debug/axes/base.hpp"
#include "tools/debug/axes/opengl.hpp"

using namespace Tool::Debug;


DebugAxes *DebugAxes::GetInstance() {
    // TODO: Add switch to select render backend
    return new OpenglDebugAxes();
}
