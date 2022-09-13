#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform vec3 transform_position;
uniform mat4 MVP;

out vec3 Normal;
out vec3 FragPos;


void main() {
    vec3 new_position = vec3(
        position.x + transform_position.x,
        position.y + transform_position.y,
        position.z + transform_position.z
    );
    FragPos = new_position;
    // TODO: Need to transpose and inverse model matrix for scaling
    Normal = normal;
    gl_Position = MVP * vec4(new_position, 1.0f);
}
