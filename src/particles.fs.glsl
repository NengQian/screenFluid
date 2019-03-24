#version 330 core

uniform mat4 uProjectionMatrix;
uniform float uSpriteSize;
uniform int uColorMode;
uniform float uNormdepth;

//in vec4 vs_color; 
in vec4 vEyeSpacePosition;
out vec4 fColor;

void main() {
	vec3 normal;

	normal.xy = 2.0 * gl_PointCoord - 1.0;
	float r1 = dot(normal.xy, normal.xy);
	if (r1 > 1.0) {
	    discard;
		//fColor = vec4(1.0,1.0,1.0,1.0);  // we set a maximum value to this fragment since it is not on particles.
	}

		// output depth in eye frame.    
    	//float depth = - vEyeSpacePosition.z/uNormdepth; // since z here is negative in eyespace...
		//fColor = vec4(depth, depth, depth, 1.0);  //  the depth will directly get from the depth buffer.. so Do nothing here.. but is this correct?

	normal.z = -sqrt(1.0 - r1);
	normal = normalize(normal);


	// // compute fragment depth
	vec4 pixelPos = vec4(vEyeSpacePosition.xyz + normal*uSpriteSize, 1.0);
	float depth = - pixelPos.z/uNormdepth; // since z here is negative in eyespace...

	fColor = vec4(depth, depth, depth, 1.0);
	// vec4 clipSpacePos = uProjectionMatrix*pixelPos;
	// float fragDepth = (clipSpacePos.z / clipSpacePos.w)/2.0 + 0.5 ; // near -1, far 1 -> near 0 far 1

	// int Mode = 3;
	// switch(Mode) {
	// case 0:
	// 	float diffuse = max(0.0, dot(normal, lightDir));
	// 	fColor = diffuse * squadColor + ambientLight;
	// 	break;
	// case 1:
	// 	fColor = vec3( fragDepth, fragDepth, fragDepth);
	// 	break;
	// case 2:
	// 	fColor = abs(normal);
	// 	break;
	// case 3:
	// 	vec4 sphereClipPos =  uProjectionMatrix*vEyeSpacePosition;
	// 	float sphereDepth = sphereClipPos.z/sphereClipPos.w;
	// 	sphereDepth = sphereDepth/2.0 + 0.5;
 	// 	fColor = vec3(sphereDepth, sphereDepth, sphereDepth);
	// 	break;
	// }

	// debug
	// fColor = vs_color.rgb;



}
