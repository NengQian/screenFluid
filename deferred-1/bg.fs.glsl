#version 330

uniform samplerCube uTexCube;

uniform mat4 uInverseViewProjection;

in vec2 vPosition;

layout(location = 0) out vec3 fColor;
layout(location = 1) out vec3 fPosition;
layout(location = 2) out vec3 fNormal;

void main() {
    vec4 near = uInverseViewProjection * vec4(vPosition, 0, 1);
    vec4 far = uInverseViewProjection * vec4(vPosition, 1, 1);

    vec3 dir = far.xyz / far.w - near.xyz / near.w;

    fColor = texture(uTexCube, dir).rgb;
    fPosition = vec3(0);
    fNormal = vec3(0);
}
