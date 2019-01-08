#version 330 core

uniform sampler2D uTexColor;

in vec3 vPosition;
in vec2 vTexCoord;
in float vProgress;

out vec3 fColor;

void main() {
    fColor = texture(uTexColor, vTexCoord).rgb;
    fColor = clamp(fColor - .05, vec3(0), vec3(1)); // bg is not perfectly black
    fColor *= smoothstep(0.0, 0.2, vProgress);
    fColor *= smoothstep(1.0, 0.6, vProgress);
}