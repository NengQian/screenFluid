#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform float uRuntime = 0.0;
uniform mat4 uModelViewProjection;

out vec3 vPosition;
out vec3 vNormal;

void main() {
    vPosition = aPosition;
    gl_Position = uModelViewProjection * vec4(aPosition, 1.0);

    vNormal = aNormal;
}