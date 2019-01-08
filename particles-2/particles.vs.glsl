#version 330 core

layout(location = 0) in vec2 aQuadCoordinate;
layout(location = 1) in vec3 aPosition;
layout(location = 2) in float aProgress;
layout(location = 3) in float aRandom;

uniform float uRuntime = 0.0;
uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 vPosition;
out vec2 vTexCoord;
out float vProgress;

void main() {
    vProgress = aProgress;
    vTexCoord = aQuadCoordinate;
    vec4 p = uModelViewMatrix * vec4(aPosition, 1.0);
    float size = 0.5;
    size *= 0.5 + 0.5 * smoothstep(0, 0.6, aProgress);
    vec2 o = (aQuadCoordinate * 2 - 1) * size;
    float a = aRandom * 6.28 + vProgress * (aRandom * .5 - .5) * 5;
    p.xy += vec2(
        cos(a) * o.x - sin(a) * o.y,
        sin(a) * o.x + cos(a) * o.y
    );
    vPosition = p.xyz;
    gl_Position = uProjectionMatrix * p;

}