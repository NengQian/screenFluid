#version 410 core

layout(triangles, fractional_even_spacing, ccw) in;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

patch in vec3 cNormal[3];

uniform bool uTess;

out vec3 tNormal;
out vec3 tWorldPos;

float vij(vec3 pi, vec3 pj, vec3 ni, vec3 nj) {
    vec3 v = pj - pi;
    return 2.0 * dot(v, ni + nj) / dot(v,  v);
}

void main()
{
    // http://alex.vlachos.com/graphics/CurvedPNTriangles.pdf
    /*vec4 viewPos =
        gl_in[0].gl_Position * gl_TessCoord.x +
        gl_in[1].gl_Position * gl_TessCoord.y +
        gl_in[2].gl_Position * gl_TessCoord.z;

    tNormal =
        cNormal[0] * gl_TessCoord.x +
        cNormal[1] * gl_TessCoord.y +
        cNormal[2] * gl_TessCoord.z;*/

    float b0 = gl_TessCoord.x;
    float b1 = gl_TessCoord.y;
    float b2 = gl_TessCoord.z;

    vec3 n0 = cNormal[0];
    vec3 n1 = cNormal[1];
    vec3 n2 = cNormal[2];

    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;

    float w01 = dot(p1 - p0, n0);
    float w02 = dot(p2 - p0, n0);
    float w10 = dot(p0 - p1, n1);
    float w12 = dot(p2 - p1, n1);
    float w20 = dot(p0 - p2, n2);
    float w21 = dot(p1 - p2, n2);

    // PN bezier triangle
    vec3 p300 = p0;
    vec3 p030 = p1;
    vec3 p003 = p2;

    vec3 p210 = (2.0 * p0 + p1 - w01 * n0) / 3.0;
    vec3 p120 = (2.0 * p1 + p0 - w10 * n1) / 3.0;

    vec3 p021 = (2.0 * p1 + p2 - w12 * n1) / 3.0;
    vec3 p012 = (2.0 * p2 + p1 - w21 * n2) / 3.0;

    vec3 p102 = (2.0 * p2 + p0 - w20 * n2) / 3.0;
    vec3 p201 = (2.0 * p0 + p2 - w02 * n0) / 3.0;

    vec3 E = (p210 + p120 + p021 + p012 + p102 + p201) / 6.0;
    vec3 V = (p0 + p1 + p2) / 3.0;
    vec3 p111 = E + (E - V) / 2;

    vec3 n200 = n0;
    vec3 n020 = n1;
    vec3 n002 = n2;

    float v01 = vij(p0, p1, n0, n1);
    float v12 = vij(p1, p2, n1, n2);
    float v20 = vij(p2, p0, n2, n0);

    vec3 n110 = normalize(n0 + n1 + v01 * (p1 - p0));
    vec3 n011 = normalize(n1 + n2 + v12 * (p2 - p1));
    vec3 n101 = normalize(n2 + n0 + v20 * (p0 - p1));

    vec3 objPos =
        p300 * (b0 * b0 * b0) +
        p030 * (b1 * b1 * b1) +
        p003 * (b2 * b2 * b2) +

        p210 * (b0 * b0 * b1 * 3.0) +
        p201 * (b0 * b0 * b2 * 3.0) +

        p120 * (b1 * b1 * b0 * 3.0) +
        p021 * (b1 * b1 * b2 * 3.0) +

        p102 * (b2 * b2 * b0 * 3.0) +
        p012 * (b2 * b2 * b1 * 3.0) +

        p111 * (b0 * b1 * b2 * 6.0);

    tNormal =
        n200 * (b0 * b0) +
        n020 * (b1 * b1) +
        n002 * (b2 * b2) +

        n110 * (b0 * b1) +
        n101 * (b0 * b2) +
        n011 * (b2 * b1);

    if (!uTess)
    {
        objPos =
            gl_in[0].gl_Position.xyz * gl_TessCoord.x +
            gl_in[1].gl_Position.xyz * gl_TessCoord.y +
            gl_in[2].gl_Position.xyz * gl_TessCoord.z;

        tNormal =
            cNormal[0] * gl_TessCoord.x +
            cNormal[1] * gl_TessCoord.y +
            cNormal[2] * gl_TessCoord.z;
    }

    tNormal = mat3(uModelMatrix) * tNormal;
    vec4 worldPos = uModelMatrix * vec4(objPos, 1.0);
    tWorldPos = worldPos.xyz;
    gl_Position = uProjectionMatrix * uViewMatrix * worldPos;
}