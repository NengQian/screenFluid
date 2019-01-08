#version 410 core

uniform sampler2D uTexColor;
uniform sampler2D uTexNormal;
uniform sampler2D uTexHeight;

layout(quads, fractional_even_spacing, ccw) in;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

out vec3 tNormal;
out vec3 tTangent;
out vec3 tWorldPos;
out vec2 tTexCoord;

void main()
{
    tTexCoord = gl_TessCoord.xy;

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    vec4 p01 = mix(p1, p0, gl_TessCoord.x);
    vec4 p23 = mix(p2, p3, gl_TessCoord.x);

    vec4 p = mix(p01, p23, gl_TessCoord.y);

    float h = texture(uTexHeight, tTexCoord).r;
    p.y += (h * 2 - 1) * .05;

    gl_Position = uProjectionMatrix * uViewMatrix * p;
    tNormal = vec3(0, 1, 0);
    tTangent = vec3(1, 0, 0);
    tWorldPos = p.xyz;
}