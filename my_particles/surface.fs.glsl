#version 330 core

uniform mat4 uProjectionMatrix;
uniform float uSpriteSize;
uniform int uColorMode;
uniform float uMaxdepth;
uniform float uNormdepth;
uniform sampler2D tex;  


//in vec4 vs_color; 
in vec4 vEyeSpacePosition;
out vec3 fColor;

// actually we waste a lot of time here.
// when we go to this surface shader, we actually only need to consider these surface's pixel, which is exactly pixel's number
// however, since we take fragement from vertex shader, we then actually do many useless computation...
// yes, I need to modify the vertex shader for smoothing and the surface...
void main() {
	vec3 normal;
	normal.xy = 2.0 * gl_PointCoord - 1.0;
	float r1 = dot(normal.xy, normal.xy);
	if (r1 > 1.0) {
	    discard;
	} 
    float depth = texelFetch(tex, ivec2(gl_FragCoord.xy),0).x;
    if(depth*uNormdepth > uMaxdepth) // means this fragment is not in fluid, but in background
        discard;
    
	fColor = vec3(depth, depth, depth);  //  the depth will directly get from the depth buffer.. so Do nothing here.. but is this correct?



	// normal.z = -sqrt(1.0 - r1);

	// vec3 lightDir = vec3(0.0, -1.0, 0.0);
	// vec3 squadColor = vec3(0.2, 0.5, 1.0);
	// vec3 ambientLight = vec3(0.1, 0.1, 0.3);

	// normal = normalize(normal);
    // float diffuse = max(0.0, dot(normal, lightDir));
	// fColor = diffuse * squadColor + ambientLight;
}