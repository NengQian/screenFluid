#version 330 core

uniform vec3 uLightPos;
uniform vec3 uCamPos;

in vec3 vNormal;
in vec3 vColor;
in vec3 vWorldPos;

out vec3 fColor;

void main() {
    // view and light direction
    vec3 l = normalize(uLightPos - vWorldPos);
    vec3 v = normalize(uCamPos - vWorldPos);
    vec3 normal = normalize(vNormal);
    vec3 r = reflect(-v, vNormal);

    vec3 baseColor = vec3(0.83, 0.18, 0.18);

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