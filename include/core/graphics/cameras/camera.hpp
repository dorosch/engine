#ifndef __GRAPHIC_CAMERAS_CAMERA__
#define __GRAPHIC_CAMERAS_CAMERA__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/graphics/cameras/exception.hpp"

namespace Engine {
    namespace Graphics {
        namespace Camera {
            enum Projection {
                PERSPECTIVE,
                ORTHOGRAPHIC
            };


            // TODO: Maybe move to some constant (in window for example)?
            struct Viewport {
                float x;
                float y;
                float width;
                float height;
            };


            class Camera {
            public:
                // TODO: Need to add roll support
                // float roll;
                float yaw;
                float pitch;

                float zoom;
                float speed;
                float sensitivity;

                glm::vec3 position;
                glm::vec3 direction;

                glm::vec3 up;
                glm::vec3 front;
                glm::vec3 right;

                Projection projection;

                Camera();
                void updateVectors();
                glm::mat4 getViewMatrix();
                glm::mat4 getProjectionMatrix();
            };
        }
    }
}

#endif
