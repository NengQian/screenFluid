#version 330 core

uniform mat4 uProjectionMatrix;
uniform float uSpriteSize;
uniform int uColorMode;
uniform float uNormdepth;

//in vec4 vs_color;
in vec4 vEyeSpacePosition;
in float vDensity;
out vec4 fColor;




void main() {

	if(vDensity<200)
		discard;

	vec3 normal;

	normal.xy = 2.0 * gl_PointCoord - 1.0;
	float r1 = dot(normal.xy, normal.xy);
	if (r1 > 1.0) {
	    discard;
	}
	else
	{



		normal.z = -sqrt(1.0 - r1);
		normal = normalize(normal);
		vec4 pixelPos = vec4(vEyeSpacePosition.xyz + normal*uSpriteSize, 1.0);

	    float depth =  -pixelPos.z/uNormdepth; // since z here is negative in eyespace...

	    fColor = vec4(depth, depth, depth, 1.0); // all in all, which depth should I send???
		
			    //vec4 clipSpacePos = uProjectionMatrix*pixelPos;
	    //gl_FragDepth = (clipSpacePos.z / clipSpacePos.w)/2.0 + 0.5 ; 

	    // vec3 lightDir = vec3(0.0, -1.0, 0.0);
	    // vec3 squadColor = vec3(0.2706, 0.2392, 0.7451);
	    // vec3 ambientLight = vec3(0.1, 0.1, 0.3);

        // float diffuse = max(0.0, dot(normal, lightDir));
	    // fColor = vec4(diffuse * squadColor + ambientLight,1.0);

	}
}
