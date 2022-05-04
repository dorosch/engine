#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    textureCoords = texCoord;
    gl_Position = projection * view * model * position;
}
