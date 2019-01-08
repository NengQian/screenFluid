#version 330 core

uniform vec3 uLightDir = normalize(vec3(2, 2, 1));

in vec3 vNormal;
in vec3 vColor;

out vec3 fColor;

void main() {
    fColor = vColor * max(dot(vNormal, uLightDir), 0);
}