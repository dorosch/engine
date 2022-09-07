#ifndef __MATH_TYPE__
#define __MATH_TYPE__

#include <glm/glm.hpp>


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texture;
};

#endif
