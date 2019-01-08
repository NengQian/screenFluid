#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform float uQuadSize = 0.05;

// output to fragment shader
out vec3 vColor;

void emitVertex(vec3 p, float dx, float dy) {
    // color from [0,0,0] to [1,1,0]
    vColor = vec3(dx * .5 + .5, dy * .5 + .5, 0);

    // calc position in camera space
    gl_Position = uViewMatrix * vec4(p, 1.0);
    // "span" a quad (camera-aligned)
    gl_Position.xy += vec2(dx, dy) * uQuadSize;
    // project into NDC
    gl_Position = uProjectionMatrix * gl_Position;
    // finalize vertex
    EmitVertex();
}

void main() {
    vec3 p = gl_in[0].gl_Position.xyz;

    emitVertex(p, -1, -1);
    emitVertex(p, 1, -1);
    emitVertex(p, -1, 1);
    emitVertex(p, 1, 1);
    EndPrimitive();
}