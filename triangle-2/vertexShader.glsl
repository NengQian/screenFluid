#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

uniform float uRuntime = 0.0;

out vec3 vPosition;
out vec3 vColor;

void main() {
    vPosition = aPosition * cos(uRuntime);
    vColor = aColor;

    gl_Position = vec4(vPosition, 1.0);
}