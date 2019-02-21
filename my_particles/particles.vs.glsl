#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform vec2 uScreenSize;
uniform float uSpriteSize;

out vec4 vEyeSpacePosition;

void main() {
    vec4 p = uModelViewMatrix * vec4(aPosition, 1.0);
    vEyeSpacePosition = p;

    vec4 projVoxel = uProjectionMatrix * vec4(uSpriteSize, uSpriteSize, p.z, p.w);
    vec2 projSize = uScreenSize * projVoxel.xy / projVoxel.w;   // here we just want to calculate the sprite size at p.z this distance... So if the z bigger, the sprite smaller.

    gl_PointSize = 0.25 * (projSize.x+projSize.y);
    gl_Position = uProjectionMatrix * p;
}
