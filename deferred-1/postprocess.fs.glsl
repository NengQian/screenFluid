#version 330

uniform sampler2DRect uTexColor;
uniform sampler2DRect uTexPosition;
uniform sampler2DRect uTexNormal;
uniform sampler2DRect uTexDepth;

uniform vec3 uLightPos;
uniform vec3 uCamPos;

uniform int uMode;

uniform mat4 uInverseViewProjection;
uniform mat4 uInverseProjection;

in vec2 vPosition;
out vec3 fColor;

void main() {
    vec2 texCoord = gl_FragCoord.xy;

    if (uMode == 6)
        texCoord = mod(texCoord * 2, vec2(800, 600));

    vec3 baseColor = texture(uTexColor, texCoord).rgb;
    vec3 pos = texture(uTexPosition, texCoord).xyz;
    vec3 normal = texture(uTexNormal, texCoord).xyz;
    float depth = texture(uTexDepth, texCoord).r;

    vec4 dpos = uInverseProjection * vec4(vPosition, depth * 2 - 1, 1);
    dpos /= dpos.w;

    switch (uMode) {
    case 1:
        fColor = baseColor;
        break;
    case 2:
        fColor = pos;
        break;
    case 3:
        fColor = normal;
        break;
    case 4:
        fColor = vec3(length(dpos.xyz) / 4.0);
        break;

    case 5:
        if (length(normal) < .1) {
            fColor = baseColor;
        } else {
            // view and light direction
            vec3 l = normalize(uLightPos - pos);
            vec3 v = normalize(uCamPos - pos);

            // prepare lighting calc
            vec3 r = reflect(-v, normal);


            // execute lighting calc
            vec3 color = vec3(0);

            // ambient
            color += baseColor * 0.2;

            // diffuse
            color += baseColor * max(0, dot(l, normal));

            // specular
            color += baseColor * max(0, pow(dot(l, r), 3.0)) * 0.3;

            fColor = color;
        }
        break;

    case 6:
        if (gl_FragCoord.x < 400)
            if (gl_FragCoord.y < 300)
                fColor = baseColor;
            else fColor = pos;
        else if (gl_FragCoord.y < 300)
            fColor = normal;
        else fColor = vec3(length(dpos.xyz) / 4.0);
        break;
    }
}
