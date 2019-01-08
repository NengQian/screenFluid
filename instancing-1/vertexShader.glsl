#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform float uRuntime = 0.0;
uniform mat4 uViewProjection;

uniform samplerBuffer uTexBuffer;

uniform int uMode = 1;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vColor;

void main() {

    vColor = vec3(1);

    int id = int(gl_InstanceID);

    vec3 p;
    vec3 d1, d2, d3;
    mat3 m;
    switch (uMode) {
        default:
        case 1:
            // original
            vPosition = aPosition;
            vNormal = aNormal;
            break;

        case 2:
            // cube grid of 20% scale
            float x = mod(id, 16) / 15.0;
            float y = mod(id / 16, 16) / 15.0;
            float z = mod(id / (16 * 16), 16) / 15.0;
            p = vec3(x, y, z);
            vPosition = aPosition * .2 + (p * 32.0 - 16.0);
            vNormal = aNormal;
            break;

        case 3:
            // complex procedural shader function
            float t = id / (16.0 * 16.0 * 16.0 - 2.0) * 3.14 * 2 + uRuntime * 0.001;
            float a = 15;
            p = vec3(
                1.5 * cos(t) - cos(15 * t),
                cos(31 * t) - sin(3 * t),
                1.5 * sin(t) - sin(23 * t)
            );
            d1 = normalize(vec3(sin(77 * t + uRuntime), 0.2, cos(7 * t + uRuntime)));
            d2 = normalize(cross(d1, vec3(1, 1, sin(-5 * t + uRuntime))));
            d3 = cross(d1, d2);
            m = mat3(d1, d2, d3);
            vPosition = m * aPosition * .05 + p * 3;
            vNormal = m * aNormal;
            break;

        case 4:
            // positions from texture buffer
            //p = texelFetch(uTexBuffer, id * 2.0 + 0).xyz;
            //vColor = texelFetch(uTexBuffer, id * 2.0 + 1).rgb;
            vPosition = aPosition * .1 + p;
            vNormal = aNormal;
            break;
    }

    gl_Position = uViewProjection * vec4(vPosition, 1.0);
    vTexCoord = aTexCoord;
}