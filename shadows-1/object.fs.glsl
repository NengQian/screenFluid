#version 330 core

uniform sampler2DRect uTexShadow;
uniform float uTexShadowSize;
uniform mat4 uShadowViewProjMatrix;
uniform float uShadowOffset;

uniform vec3 uLightPos;

in vec3 vObjectPos;
in vec3 vWorldPos;
in vec3 vViewPos;
in vec3 vNormal;

out vec3 fColor;

void main() {
    vec3 baseColor = vec3(0.902, 0.290, 0.098);

    // shadow calculation
    vec4 shadowPos = uShadowViewProjMatrix * vec4(vWorldPos, 1.0);
    shadowPos.xyz /= shadowPos.w;
    float shadowDepth = texture(uTexShadow, (shadowPos.xy * .5 + .5) * uTexShadowSize).r;
    float refDepth = shadowPos.z * .5 + .5;
    float shadowFactor = float(refDepth <= shadowDepth + uShadowOffset);
    if (shadowPos.w < 0.0f) // fix "behind-the-light"
        shadowFactor = 1;

    fColor = (dot(normalize(vNormal), normalize(uLightPos - vWorldPos)) * shadowFactor + 0.1) * baseColor;
}