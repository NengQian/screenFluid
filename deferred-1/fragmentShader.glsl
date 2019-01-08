#version 330 core

uniform sampler2D uTexColor;
uniform sampler2D uTexNormal;

in vec3 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec2 vTexCoord;

layout(location = 0) out vec3 fColor;
layout(location = 1) out vec3 fPosition;
layout(location = 2) out vec3 fNormal;

void main() {
    // build tbn matrix
    vec3 normal = normalize(vNormal);
    vec3 tangent = normalize(vTangent);
    vec3 bitangent = normalize(cross(tangent, normal));
    mat3 tbn = mat3(tangent, bitangent, normal);

    // apply normal map
    vec3 normalMap = texture(uTexNormal, vTexCoord).xyz;
    normalMap.xy = normalMap.xy * 2 - 1; // project xy properly
    normal = normalize(tbn * normalMap);

    vec3 baseColor = texture(uTexColor, vTexCoord).rgb;

    fColor = baseColor;
    fPosition = vPosition;
    fNormal = normal;
}