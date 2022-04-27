#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

uniform mat4 MVP;


void main() {
    ourColor = color;
    gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0f);
}
