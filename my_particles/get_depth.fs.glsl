#version 330 core

uniform mat4 uProjectionMatrix;
uniform float uSpriteSize;
uniform int uColorMode;
uniform float uNormdepth;

in vec4 vEyeSpacePosition;
out vec4 fColor;

void main() {
	vec3 normal;

	normal.xy = 2.0 * gl_PointCoord - 1.0;
	float r1 = dot(normal.xy, normal.xy);
	if (r1 > 1.0) {
	    discard;
	}
	else
	{
		// output depth in eye frame.    
    	float depth = - vEyeSpacePosition.z/uNormdepth; // since z here is negative in eyespace...
		fColor = vec4(depth, depth, depth, 1.0);  //  the depth will directly get from the depth buffer.. so Do nothing here.. but is this correct?
	}
}
