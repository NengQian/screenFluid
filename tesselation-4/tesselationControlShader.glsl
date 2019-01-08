#version 410 core

layout(vertices = 4) out;

uniform float uRuntime = 0.0;

void main(void)
{
    gl_TessLevelOuter[0] =
    gl_TessLevelOuter[1] =
    gl_TessLevelOuter[2] =
    gl_TessLevelOuter[3] =
    gl_TessLevelInner[0] =
    gl_TessLevelInner[1] = 64;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}