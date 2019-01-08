#version 330

uniform sampler2DRect uTexColor;

uniform bool uBlur;

in vec2 vPosition;
out vec3 fColor;

void main() {
    vec2 texCoord = gl_FragCoord.xy;

    vec3 origColor = texture(uTexColor, texCoord).rgb;

    vec2 dir = vec2(1,0);
    vec3 color =
        0.006 * texture(uTexColor, texCoord - 3 * dir).rgb +
        0.061 * texture(uTexColor, texCoord - 2 * dir).rgb +
        0.242 * texture(uTexColor, texCoord - 1 * dir).rgb +
        0.383 * texture(uTexColor, texCoord).rgb +
        0.242 * texture(uTexColor, texCoord + 1 * dir).rgb +
        0.061 * texture(uTexColor, texCoord + 2 * dir).rgb +
        0.006 * texture(uTexColor, texCoord + 3 * dir).rgb;

    fColor = uBlur ? color : origColor;
}
