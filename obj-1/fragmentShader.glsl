#version 330 core

uniform vec3 uLightPos;
uniform vec3 uCamPos;

in vec4 vPosition;
in vec3 vNormal;

out vec3 fColor;

void main() {
    // view and light direction
    vec3 l = normalize(uLightPos - vPosition.xyz);
    vec3 v = normalize(uCamPos - vPosition.xyz);
    vec3 normal = vNormal * sign(dot(v, vNormal));

    // prepare lighting calc
    vec3 r = reflect(-v, normal);

    vec3 baseColor = vec3(0.3);

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