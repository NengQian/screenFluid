#version 330

uniform sampler2DRect uTexColor;
uniform sampler2DRect uTexDepth;

out vec3 fColor;

void main() {
    vec2 texCoord = gl_FragCoord.xy;

    vec3 color = texture(uTexColor, texCoord).rgb;
    float depth = texture(uTexDepth, texCoord).r;

    // invert BG, suffle FG
    if (depth > 0.99)
        fColor = color.rbg;
    else fColor = vec3(1) - color;
}
