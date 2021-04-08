#version 330 core

layout(location = 0) in vec3 pos;

out vec3 position;

void main() {
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);

    position = pos;
}