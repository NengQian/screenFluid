#version 330 core

uniform sampler2D uTexColor;
uniform sampler2D uTexNormal;
uniform sampler2D uTexHeight;

uniform vec3 uLightPos;
uniform vec3 uCamPos;

uniform int uMode = 1;

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

    // apply parallax map
    vec2 texCoord = vTexCoord;
    float parallaxScale = 0.05;
    const int samples = 16;
    vec2 texViewDir = -normalize(transpose(tbn) * v).xy;
    vec2 texOffset = vec2(0.0);
    for (int i = 0; i < samples; ++i) {
        float height = 1 - texture(uTexHeight, texCoord + texOffset).x;
        texOffset = mix(texOffset, texViewDir * parallaxScale * height, 2.0 / samples);
    }
    texCoord += texOffset;
    // MODE 5+6: parallax map
    if (uMode < 5)
        texCoord = vTexCoord;

    // apply normal map
    vec3 normalMap = texture(uTexNormal, texCoord).xyz;
    normalMap.xy = normalMap.xy * 2 - 1; // project xy properly
    normal = normalize(tbn * normalMap);
    // MODE 3: normal mapping
    if (uMode < 3)
        normal = normalize(vNormal);

    // prepare lighting calc
    vec3 r = reflect(-v, normal);

    vec3 baseColor = texture(uTexColor, texCoord).rgb;

    // execute lighting calc
    vec3 color = vec3(0);

    // ambient
    color += baseColor * 0.2;

    // diffuse
    color += baseColor * max(0, dot(l, normal));

    // specular
    color += baseColor * max(0, pow(dot(l, r), 3.0)) * 0.3;

    fColor = color;

    // MODE 1: color only
    if (uMode == 1)
        fColor = baseColor;

    // MODE 3: normal only
    if (uMode == 3 || uMode == 6)
        fColor = normal;
}