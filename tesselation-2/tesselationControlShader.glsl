#version 410 core

layout(vertices = 3) out;

uniform float uRuntime = 0.0;

in vec3 vNormal[];
patch out vec3 cNormal[3];

void main(void)
{
    gl_TessLevelOuter[0] =
    gl_TessLevelOuter[1] =
    gl_TessLevelOuter[2] =
    gl_TessLevelInner[0] = 8.0 + 6.0 * cos(uRuntime);

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    cNormal[0] = vNormal[0];
    cNormal[1] = vNormal[1];
    cNormal[2] = vNormal[2];
}