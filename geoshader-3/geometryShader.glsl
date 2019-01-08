#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 21) out;

uniform float uRuntime = 0.0;
uniform mat4 uModelMatrix;
uniform mat4 uViewProjectionMatrix;

uniform float uInsetScale = 0.6;
uniform float uExtrusion = 0.08;

// output to fragment shader
out vec3 vColor;
out vec3 vNormal;
out vec3 vWorldPos;

vec3 inset(vec3 p, vec3 center, vec3 n) {
    return center + (p - center) * uInsetScale + n * uExtrusion;
}

vec3 normalOf(vec3 p0, vec3 p1, vec3 p2) {
    vec3 v0 = p1 - p0;
    vec3 v1 = p2 - p0;

    return normalize(cross(v0, v1));
}

void emitQuad(vec3 p0, vec3 p1, vec3 p2, vec3 p3) {
    gl_Position = uViewProjectionMatrix * vec4(p0, 1.0);
    vWorldPos = p0;
    EmitVertex();
    gl_Position = uViewProjectionMatrix * vec4(p1, 1.0);
    vWorldPos = p1;
    EmitVertex();
    gl_Position = uViewProjectionMatrix * vec4(p3, 1.0);
    vWorldPos = p3;
    EmitVertex();
    gl_Position = uViewProjectionMatrix * vec4(p2, 1.0);
    vWorldPos = p2;
    EmitVertex();
    EndPrimitive();
}

void main() {
    // per-triangle calculation
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 center = (p0 + p1 + p2) / 3.0;

    vec3 n = normalOf(p0, p1, p2);

    vec3 pi0 = inset(p0, center, n);
    vec3 pi1 = inset(p1, center, n);
    vec3 pi2 = inset(p2, center, n);

    vec3 n01 = normalOf(p0,  p1,  pi1);
    vec3 n12 = normalOf(p1,  p2,  pi2);
    vec3 n20 = normalOf(p2,  p0,  pi0);

    // constant color
    vColor = vec3(0.3);

    // center triangle
    vNormal = n;
    for (int i = 0; i < 3; ++i) {
        vWorldPos = inset(gl_in[i].gl_Position.xyz, center, n);
        gl_Position = uViewProjectionMatrix * vec4(vWorldPos, 1.0);
        EmitVertex();
    }
    EndPrimitive();

    // Quads
    vNormal = n01;
    emitQuad(p0, p1, pi1, pi0);
    vNormal = n12;
    emitQuad(p1, p2, pi2, pi1);
    vNormal = n20;
    emitQuad(p2, p0, pi0, pi2);
}