#version 410 core

layout(vertices = 3) out;

uniform float uRuntime = 0.0;

void main(void)
{
    gl_TessLevelOuter[0] = 64.0;
    gl_TessLevelOuter[1] = 64.0;
    gl_TessLevelOuter[2] = 64.0;

    gl_TessLevelInner[0] = 64.0;// + 6.0 * cos(uRuntime );
    //gl_TessLevelOuter[0] = 8.0 + 6.0 * cos(uRuntime / 10);

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}