#ifndef __GRAPHIC_CAMERAS_CAMERA__
#define __GRAPHIC_CAMERAS_CAMERA__

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/object/object.hpp"
#include "core/ecs/component.hpp"
#include "core/graphics/cameras/type.hpp"
#include "core/graphics/cameras/exception.hpp"

namespace Engine {
    namespace Graphics {
        namespace Camera {
            class Camera : public Object {
            public:
                Camera();
                void updateVectors();
                glm::mat4 getViewMatrix();
                glm::mat4 getProjectionMatrix();
            };
        }
    }
}

#endif
