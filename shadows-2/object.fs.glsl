#version 330 core

uniform sampler2DRect uTexShadow;
uniform float uTexShadowSize;
uniform mat4 uShadowViewProjMatrix;
uniform mat4 uShadowViewMatrix;
uniform float uShadowOffset;

uniform float uShadowRange = 50;
uniform float uShadowConstant = 80;

uniform vec3 uLightPos;

in vec3 vObjectPos;
in vec3 vWorldPos;
in vec3 vViewPos;
in vec3 vNormal;

out vec3 fColor;

void main() {
    vec3 baseColor = vec3(0.902, 0.290, 0.098);

    // shadow calculation
    vec4 shadowViewPos = uShadowViewMatrix * vec4(vWorldPos, 1.0);
    vec4 shadowPos = uShadowViewProjMatrix * vec4(vWorldPos, 1.0);
    shadowPos.xyz /= shadowPos.w;
    float shadowExpDepth = texture(uTexShadow, (shadowPos.xy * .5 + .5) * uTexShadowSize).r;
    float z = -shadowViewPos.z / uShadowRange;
    float refExpDepth = exp(-z * uShadowConstant);
    float shadowFactor = clamp(refExpDepth * shadowExpDepth, 0, 1);

    if (shadowPos.w < 0.0f || z > 1) // fix "behind-the-light"
        shadowFactor = 1;

    fColor = (dot(normalize(vNormal), normalize(uLightPos - vWorldPos)) * shadowFactor + 0.1) * baseColor;
}