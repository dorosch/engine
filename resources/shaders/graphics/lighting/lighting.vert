#version 330 core

layout (location = 0) in vec3 position;

uniform vec3 transform_position;
uniform mat4 MVP;


void main() {
    gl_Position = MVP * vec4(
        position.x + transform_position.x,
        position.y + transform_position.y,
        position.z + transform_position.z,
        1.0
    );
}
