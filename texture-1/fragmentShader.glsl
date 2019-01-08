#version 330 core

uniform sampler2D uTexture;

in vec3 vPosition;
in vec2 vTexCoord;

out vec3 fColor;

void main() {
    fColor = texture(uTexture, vTexCoord).rgb;
}