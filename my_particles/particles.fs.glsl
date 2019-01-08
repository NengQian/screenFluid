#version 330 core

uniform sampler2D uTexColor;

in vec3 vPosition;
in vec2 vTexCoord;

out vec3 fColor;

void main() {
    fColor = texture(uTexColor, vTexCoord).rgb;
    fColor = clamp(fColor - .05, vec3(0), vec3(1)); // bg is not perfectly black
}
