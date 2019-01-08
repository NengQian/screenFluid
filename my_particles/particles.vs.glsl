#version 330 core

layout(location = 0) in vec2 aQuadCoordinate;
layout(location = 1) in vec3 aPosition;


uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

out vec2 vTexCoord;

void main() {
    // project into view space
    vec4 p = uModelViewMatrix * vec4(aPosition, 1.0);
    // apply quad geometry
    p.xy += (aQuadCoordinate * 2 - 1) * .5;
    // project into NDC
    gl_Position = uProjectionMatrix * p;

    vTexCoord = aQuadCoordinate;
}
