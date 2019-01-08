#version 330 core

uniform float uShadowRange = 50;
uniform float uShadowConstant = 80;

in vec3 vObjectPos;
in vec3 vWorldPos;
in vec3 vViewPos;

out float fShadow;

void main() {
    float z = -vViewPos.z / uShadowRange;
    fShadow = exp(z * uShadowConstant);
}