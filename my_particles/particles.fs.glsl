#version 330 core

uniform mat4 uProjectionMatrix;
uniform float uSpriteSize;
uniform int uColorMode;

 in vec4 vEyeSpacePosition;
out vec3 fColor;
//layout (depth_any) out float gl_FragDepth;

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
	// vec4 clipSpacePos = pixelPos * uProjectionMatrix;
	// float fragDepth = clipSpacePos.z / clipSpacePos.w;
	vec4 clipSpacePos = uProjectionMatrix*pixelPos;
	float fragDepth = (clipSpacePos.z / clipSpacePos.w)/2.0 + 0.5 ; // near -1, far 1 -> near 0 far 1
	//gl_FragDepth = fragDepth;


	int Mode = 3;
	switch(Mode) {
	case 0:
		float diffuse = max(0.0, dot(normal, lightDir));
		fColor = diffuse * squadColor + ambientLight;
		break;
	case 1:
		fColor = vec3( fragDepth, fragDepth, fragDepth);
		break;
	case 2:
		fColor = abs(normal);
		break;
	case 3:
		vec4 sphereClipPos =  uProjectionMatrix*vEyeSpacePosition;
		float sphereDepth = sphereClipPos.z/sphereClipPos.w;
		sphereDepth = sphereDepth/2.0 + 0.5;
 		fColor = vec3(sphereDepth, sphereDepth, sphereDepth);
		break;
	}


}
