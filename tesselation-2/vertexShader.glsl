#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;

out vec3 vNormal;

void main() {
    gl_Position = uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
    vNormal = mat3(uModelMatrix) * aNormal;
}