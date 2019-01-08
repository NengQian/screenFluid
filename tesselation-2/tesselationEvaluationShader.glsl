#version 410 core

layout(triangles, fractional_even_spacing, ccw) in;

uniform mat4 uProjectionMatrix;

patch in vec3 cNormal[3];

out vec3 tNormal;

void main()
{
    vec4 viewPos =
        gl_in[0].gl_Position * gl_TessCoord.x +
        gl_in[1].gl_Position * gl_TessCoord.y +
        gl_in[2].gl_Position * gl_TessCoord.z;

    tNormal =
        cNormal[0] * gl_TessCoord.x +
        cNormal[1] * gl_TessCoord.y +
        cNormal[2] * gl_TessCoord.z;

    gl_Position = uProjectionMatrix * viewPos;
}