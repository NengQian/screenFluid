#version 330 core

layout(location = 0) in vec3 aPosition;

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;

void main() {
    gl_Position = uModelMatrix * vec4(aPosition, 1.0);
}