#version 330 core

const vec2 positions[4] = vec2[4](
    vec2(-1, -1),
    vec2( 1, -1),
    vec2( 1,  1),
    vec2(-1,  1)
);

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;

void main() {
    vec2 coord = positions[gl_VertexID];
    gl_Position = uModelMatrix * vec4(coord.x, 0, coord.y, 1.0);
}