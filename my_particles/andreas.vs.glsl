#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in float density;
uniform vec3 color = vec3(0.0, 0.0, 0.5);
uniform float radius;

out VertexData
{
    vec3 sphere_color;
    float sphere_radius;
} vs_out;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 view_pos = view * vec4(pos, 1.0);
    vs_out.sphere_radius = radius;
    vs_out.sphere_color = color;
    gl_Position = view_pos;
}