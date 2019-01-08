#version 330

uniform sampler2DRect uTexDepth;

in vec2 vPosition;
out float fDepth;

void main() {
    vec2 texCoord = gl_FragCoord.xy;

    vec2 dir = vec2(0,1);
    float d =
        0.006 * texture(uTexDepth, texCoord - 3 * dir).r +
        0.061 * texture(uTexDepth, texCoord - 2 * dir).r +
        0.242 * texture(uTexDepth, texCoord - 1 * dir).r +
        0.383 * texture(uTexDepth, texCoord).r +
        0.242 * texture(uTexDepth, texCoord + 1 * dir).r +
        0.061 * texture(uTexDepth, texCoord + 2 * dir).r +
        0.006 * texture(uTexDepth, texCoord + 3 * dir).r;

    fDepth = d;
}
