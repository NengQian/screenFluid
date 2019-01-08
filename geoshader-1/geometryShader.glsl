#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;
uniform mat4 uViewProjectionMatrix;

// input _arrays_ (size 3)
in vec3 gColor[3];

// output to fragment shader
out vec3 vColor;
out vec3 vNormal;

void main() {
    // per-triangle calculation
    vec3 v0 = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 v1 = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 n = normalize(cross(v0, v1));

    // vertices
    for (int i = 0; i < 3; ++i) {
        vColor = gColor[i];
        vNormal = n;
        gl_Position = uViewProjectionMatrix * gl_in[i].gl_Position;
        EmitVertex();
    }

    // emit triangle
    EndPrimitive();
}