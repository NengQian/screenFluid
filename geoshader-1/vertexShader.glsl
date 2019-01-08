#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;
uniform mat4 uViewProjectionMatrix;

out vec3 gColor;

void main() {
    gl_Position = uModelMatrix * vec4(aPosition, 1.0);

    gColor = aColor;
}