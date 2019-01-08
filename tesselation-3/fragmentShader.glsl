#version 330 core

uniform vec3 uLightDir = normalize(vec3(2, 2, 1));
uniform vec3 uCamPos;

in vec3 tNormal;
in vec3 tWorldPos;

out vec3 fColor;

void main() {
    vec3 v = normalize(uCamPos - tWorldPos);
    vec3 l = uLightDir;
    vec3 normal = normalize(tNormal);
    vec3 r = reflect(-v, normal);

    vec3 baseColor = vec3(1.00, 1, 1.00);

    // execute lighting calc
    vec3 color = vec3(0);

    // ambient
    color += baseColor * 0.2;

    // diffuse
    color += baseColor * max(0, dot(l, normal));

    // specular
    color += baseColor * max(0, pow(dot(l, r), 3.0)) * 0.3;

    fColor = color;
    //fColor = abs(normalize(tNormal));
}