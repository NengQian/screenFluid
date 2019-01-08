#version 330 core

uniform sampler2D uTexColor;
uniform sampler2D uTexNormal;
uniform samplerCube uTexCube;

uniform vec3 uLightPos;
uniform vec3 uCamPos;
uniform int uMode;

in vec3 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec2 vTexCoord;

out vec3 fColor;

void main() {
    // view and light direction
    vec3 l = normalize(uLightPos - vPosition);
    vec3 v = normalize(uCamPos - vPosition);

    // build tbn matrix
    vec3 normal = normalize(vNormal);
    vec3 tangent = normalize(vTangent);
    vec3 bitangent = normalize(cross(tangent, normal));
    mat3 tbn = mat3(tangent, bitangent, normal);

    // apply normal map
    vec3 normalMap = texture(uTexNormal, vTexCoord).xyz;
    normalMap.xy = normalMap.xy * 2 - 1; // project xy properly
    normal = normalize(tbn * normalMap);

    // prepare lighting calc
    vec3 r = reflect(-v, vNormal);
    if (uMode == 1)
        r = reflect(-v, normal);

    vec3 baseColor = texture(uTexColor, vTexCoord).rgb;

    // execute lighting calc
    vec3 color = vec3(0);

    // ambient
    color += baseColor * 0.2;

    // diffuse
    color += baseColor * max(0, dot(l, normal));

    // specular
    color += baseColor * max(0, pow(dot(l, r), 3.0)) * 0.3;

    // cubemap
    vec3 reflColor = texture(uTexCube, r).rgb;
    if (uMode == 2)
        color += reflColor;

    fColor = color;
}