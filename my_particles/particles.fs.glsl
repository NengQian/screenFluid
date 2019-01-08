#version 330 core

in vec2 vTexCoord;

out vec3 fColor;

void main() {
    fColor = vec3(vTexCoord, 0);
}