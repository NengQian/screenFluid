#version 330 core

uniform sampler2D uTexColor;
uniform sampler2D uTexNormal;
uniform sampler2D uTexHeight;

uniform vec3 uLightPos;
uniform vec3 uCamPos;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vColor;

out vec3 fColor;

void main() {
    // prepare lighting calc
    vec3 normal = normalize(vNormal);
    vec3 l = normalize(uLightPos - vPosition);
    vec3 v = normalize(uCamPos - vPosition);
    vec3 r = reflect(-v, normal);

    vec3 baseColor = mix(vec3(0.2), vec3(0.4),
        mod(int(vTexCoord.x * 8) + int(vTexCoord.y * 8), 2)) * vColor;

    // execute lighting calc
    vec3 color = vec3(0);

    // ambient
    color += baseColor * 0.2;

    // diffuse
    color += baseColor * max(0, dot(l, normal));

    // specular
    color += baseColor * max(0, pow(dot(l, r), 3.0)) * 0.3;

    fColor = color;
}