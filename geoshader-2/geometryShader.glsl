#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;
uniform mat4 uViewProjectionMatrix;

uniform float uInsetScale = 0.8;

// output to fragment shader
out vec3 vColor;
out vec3 vNormal;

void main() {
    // per-triangle calculation
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 center = (p0 + p1 + p2) / 3.0;

    vec3 v0 = p1 - p0;
    vec3 v1 = p2 - p0;

    vec3 n = normalize(cross(v0, v1));



    // vertices
    for (int i = 0; i < 3; ++i) {
        vColor = vec3(0.3);
        vNormal = n;
        gl_Position = uViewProjectionMatrix * vec4(center + (gl_in[i].gl_Position.xyz - center) * uInsetScale, 1.0);
        EmitVertex();
    }

    // emit triangle
    EndPrimitive();
}