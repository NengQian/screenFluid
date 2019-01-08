#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

out vec3 vObjectPos;
out vec3 vWorldPos;
out vec3 vViewPos;
out vec3 vNormal;

void main() {
    vObjectPos = aPosition;
    vec4 worldPos = uModelMatrix * vec4(aPosition, 1.0);
    vec4 viewPos = uViewMatrix * worldPos;
    gl_Position = uProjectionMatrix * viewPos;

    vViewPos = viewPos.xyz;
    vWorldPos = worldPos.xyz;

    // asumming non-non-uniform scaling
    vNormal = mat3(uModelMatrix) * aNormal;
}