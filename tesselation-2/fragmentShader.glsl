#version 330 core

uniform vec3 uLightDir = normalize(vec3(2, 2, 1));

in vec3 tNormal;

out vec3 fColor;

void main() {
    fColor = vec3(0.3) * max(dot(tNormal, uLightDir), 0);
    fColor = abs(tNormal);
}