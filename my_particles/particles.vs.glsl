#version 330 core

layout(location = 0) in vec2 aQuadCoordinate;
layout(location = 1) in vec3 aPosition;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 vPosition;
out vec2 vTexCoord;

void main() {
    vTexCoord = aQuadCoordinate;
    vec4 p = uModelViewMatrix * vec4(aPosition, 1.0);
    p.xy += (aQuadCoordinate * 2 - 1) * .1;
    vPosition = p.xyz;
    gl_Position = uProjectionMatrix * p;
}
