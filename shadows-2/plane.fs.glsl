#version 330 core

uniform sampler2DRect uTexShadow;
uniform float uTexShadowSize;
uniform mat4 uShadowViewProjMatrix;
uniform mat4 uShadowViewMatrix;

uniform vec3 uLightPos;


uniform float uShadowRange = 50;
uniform float uShadowConstant = 80;

in vec3 vObjectPos;
in vec3 vWorldPos;
in vec3 vViewPos;
in vec3 vNormal;

out vec3 fColor;

void main() {
    vec3 baseColor = mix(vec3(0, .4, 0), vec3(0, .6, 0),
        mod(int(floor(vWorldPos.x * 2)) + int(floor(vWorldPos.z * 2)), 2));

    // shadow calculation
    vec4 shadowViewPos = uShadowViewMatrix * vec4(vWorldPos, 1.0);
    vec4 shadowPos = uShadowViewProjMatrix * vec4(vWorldPos, 1.0);
    shadowPos.xyz /= shadowPos.w;
    float shadowExpDepth = texture(uTexShadow, (shadowPos.xy * .5 + .5) * uTexShadowSize).r;
    float z = -shadowViewPos.z / uShadowRange;
    float refExpDepth = exp(-z * uShadowConstant);
    float shadowFactor = clamp(refExpDepth * shadowExpDepth, 0, 1);

    if (shadowPos.w < 0.0f || max(abs(shadowPos.x), abs(shadowPos.y)) > 1) // fix "behind-the-light"
        shadowFactor = 1;

    fColor = (dot(normalize(vNormal), normalize(uLightPos - vWorldPos)) * shadowFactor + 0.1) * baseColor;
    //fColor = texture(uTexShadow, fract(vWorldPos.xz / 2) * uTexShadowSize).rrr;

    //fColor *= vec3(fract(shadowPos.xy * .5 + .5), 0);
    //fColor = vec3(fract(log(shadowExpDepth) / 80));
    //fColor.xy = fract((shadowPos.xy * .5 + .5));
}