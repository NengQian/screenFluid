#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;
uniform mat4 uViewProjectionMatrix;

out vec4 vPosition;
out vec3 vNormal;

void main() {
    vPosition = uModelMatrix * vec4(aPosition, 1.0);
    vNormal = aNormal;
    gl_Position = uViewProjectionMatrix * vPosition;
}