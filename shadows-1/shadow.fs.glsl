#version 330 core

uniform float uShadowRange = 20;

in vec3 vObjectPos;
in vec3 vWorldPos;
in vec3 vViewPos;

out float fShadow;

void main() {
    fShadow = -vViewPos.z / uShadowRange;
}