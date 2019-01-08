#version 330

uniform samplerCube uTexCube;

uniform mat4 uInverseViewProjection;

in vec2 vPosition;
out vec3 fColor;

void main() {
    vec4 near4 = uInverseViewProjection * vec4(vPosition, 0, 1);
    vec4 far4 = uInverseViewProjection * vec4(vPosition, 1, 1);

    vec3 near = near4.xyz / near4.w;
    vec3 far = far4.xyz / far4.w;

    vec3 dir = far - near;

    fColor = texture(uTexCube, dir).rgb;
}
