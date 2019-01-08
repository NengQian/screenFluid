#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoord;

uniform float uRuntime = 0.0;
uniform mat4 uViewProjection;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vTangent;
out vec2 vTexCoord;

void main() {
    vPosition = aPosition;
    vNormal = aNormal;
    vTangent = aTangent;
    gl_Position = uViewProjection * vec4(aPosition, 1.0);

    vTexCoord = aTexCoord;
}