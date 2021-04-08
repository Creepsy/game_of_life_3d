#version 330 core

in vec3 position;

out vec4 FragColor;

void main() {
    FragColor = vec4((position.x + 1.0) / 2.0, (position.y + 1.0) / 2.0, (position.z + 1.0) / 2.0, 1.0);
}