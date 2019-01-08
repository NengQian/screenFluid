#version 410 core

layout(triangles, fractional_even_spacing, ccw) in;

void main()
{
    vec3 bary = gl_TessCoord.xyz;
    gl_Position =
        gl_in[0].gl_Position * bary.x +
        gl_in[1].gl_Position * bary.y +
        gl_in[2].gl_Position * bary.z;
}