#version 330

const vec2 positions[6] = vec2[6](
    vec2(-1, -1),
    vec2( 1, -1),
    vec2( 1,  1),

    vec2(-1, -1),
    vec2( 1,  1),
    vec2(-1,  1)
);

out vec2 vPosition;

void main() {
    vPosition = positions[int(gl_VertexID)];
    gl_Position = vec4(vPosition, 0, 1);
}
