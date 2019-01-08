#version 330 core

uniform sampler2D uTexColor;
uniform sampler2D uTexNormal;
uniform sampler2D uTexHeight;

uniform vec3 uLightPos;
uniform vec3 uCamPos;

in vec3 tNormal;
in vec3 tTangent;
in vec3 tWorldPos;
in vec2 tTexCoord;

out vec3 fColor;

void main() {
    // view and light direction
    vec3 l = normalize(uLightPos - tWorldPos);
    vec3 v = normalize(uCamPos - tWorldPos);

    // build tbn matrix
    vec3 normal = normalize(tNormal);
    vec3 tangent = normalize(tTangent);
    vec3 bitangent = normalize(cross(tangent, normal));
    mat3 tbn = mat3(tangent, bitangent, normal);

    // apply normal map
    vec3 normalMap = texture(uTexNormal, tTexCoord).xyz;
    normalMap.xy = normalMap.xy * 2 - 1; // project xy properly
    normal = normalize(tbn * normalMap);

    // prepare lighting calc
    vec3 r = reflect(-v, normal);

    vec3 baseColor = texture(uTexColor, tTexCoord).rgb;

    // execute lighting calc
    vec3 color = vec3(0);

    // ambient
    color += baseColor * 0.2;

    // diffuse
    color += baseColor * max(0, dot(l, normal));

    // specular
    color += baseColor * max(0, pow(dot(l, r), 5.0)) * 0.5;

    fColor = color;
}