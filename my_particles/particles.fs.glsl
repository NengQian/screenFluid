#version 330 core

uniform mat4 uProjectionMatrix;
uniform float uSpriteSize;
uniform int uColorMode;

in vec4 vEyeSpacePosition;
out vec3 fColor;

void main() {
	vec3 normal;

	normal.xy = 2.0 * gl_PointCoord - 1.0;
	float r1 = dot(normal.xy, normal.xy);
	if (r1 > 1.0) {
	    discard;
	}

	normal.z = -sqrt(1.0 - r1);

	vec3 lightDir = vec3(0.0, -1.0, 0.0);
	vec3 squadColor = vec3(0.2, 0.5, 1.0);
	vec3 ambientLight = vec3(0.1, 0.1, 0.3);

	normal = normalize(normal);



	// visualize normal map
	//fColor = abs(normal);

	// compute fragment depth
	vec4 pixelPos = vec4(vEyeSpacePosition.xyz + normal*uSpriteSize, 1.0);
	vec4 clipSpacePos = pixelPos * uProjectionMatrix;
	float fragDepth = clipSpacePos.z / clipSpacePos.w;
	//fColor = vec3(1. - fragDepth*0.1, 1. - fragDepth*0.1, 1. - fragDepth*0.1);

	// visualize color
	//float diffuse = max(0.0, dot(normal, lightDir));
	//fColor = diffuse * squadColor + ambientLight;


	switch(uColorMode) {
	case 0:
		float diffuse = max(0.0, dot(normal, lightDir));
		fColor = diffuse * squadColor + ambientLight;
		break;
	case 1:
		fColor = vec3(1. - fragDepth*0.1, 1. - fragDepth*0.1, 1. - fragDepth*0.1);
		break;
	case 2:
		fColor = abs(normal);
	}
}
