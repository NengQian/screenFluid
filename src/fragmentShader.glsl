#version 330 core

in vec3 vPosition;
in vec3 vNormal;

out vec3 fColor;

void main() {
    vec3 baseColor = mix(vec3(.7, .7, .7), vec3(1.0, 1.0, 1.0),
        mod(int(floor(vPosition.x * 2)) + int(floor(vPosition.z * 2)), 2));
    fColor = dot(normalize(vNormal), normalize(vec3(0, 10, 0) - vPosition)) * baseColor;
}
