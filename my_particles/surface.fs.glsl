#version 330 core

uniform mat4 uProjectionMatrix;
uniform float uSpriteSize;
uniform vec2 uScreenSize;
uniform float uMaxdepth;
uniform float uNormdepth;
uniform sampler2D tex;  


in vec4 vEyeSpacePosition;
out vec3 fColor;


// actually we waste a lot of time here.
// when we go to this surface shader, we actually only need to consider these surface's pixel, which is exactly pixel's number
// however, since we take fragement from vertex shader, we then actually do many useless computation...
// yes, I need to modify the vertex shader for smoothing and the surface...
// now I optimized, but then the background disapper...

// the frag_coord we get here has an origin in lower_left.
vec3 uvToEye(vec2 frag_coord, float depth)
{
    float fx = 1.0;
    float fy = 1.0;  //focal length, since our near plan in perspective set as 1, so it is 1
    float posx = (2.0*frag_coord.x / uScreenSize.x - 1.0)*depth; 
    float posy = (2.0*frag_coord.y/uScreenSize.y - 1.0)*depth;
    vec3 eyePos = vec3(0.0);
    eyePos.z = depth;
    eyePos.x = posx;
    eyePos.y = posy;
    return eyePos;
}

vec3 getEyePos(vec2 texcoord)
{
    float depth = texelFetch(tex, ivec2(texcoord.xy),0).x*uNormdepth;
    return uvToEye(texcoord, depth);
}

void main() {
	vec3 normal;
	normal.xy = 2.0 * gl_PointCoord - 1.0;
	float r1 = dot(normal.xy, normal.xy);
	if (r1 > 1.0) {
	    discard;
	} 
    float depth = texelFetch(tex, ivec2(gl_FragCoord.xy),0).x*uNormdepth;

    if(depth > uMaxdepth) // means this fragment is not in fluid, but in background
        discard;

    //     float visDepth = depth*5 -5;
    // fColor = vec3(visDepth,visDepth,visDepth);
    
    vec4 projVoxel = uProjectionMatrix * vec4(uSpriteSize, uSpriteSize, depth, 1.0);
    vec2 projSize = uScreenSize * projVoxel.xy / projVoxel.w;
    //float texelSize = 0.5*(projSize.x+projSize.y);
    float texelSize = 1.0;

    vec3 posEye = uvToEye(gl_FragCoord.xy, depth);

    vec3 ddx =  getEyePos(gl_FragCoord.xy + vec2(texelSize,0.0)) - posEye;
    vec3 ddx2 = posEye - getEyePos(gl_FragCoord.xy + vec2(-texelSize,0.0));
    if(abs(ddx.z)>abs(ddx2.z)){
        ddx = ddx2;
    }

    vec3 ddy = getEyePos(gl_FragCoord.xy + vec2(0.0, texelSize)) - posEye;
    vec3 ddy2 = posEye - getEyePos(gl_FragCoord.xy + vec2(0.0, -texelSize));
    if(abs(ddy.z)>abs(ddy2.z)){
        ddy = ddy2;
    }

    vec3 n = cross(ddx,ddy);
    n = normalize(n);
    fColor = abs(n);





	vec3 lightDir = vec3(0.0, -1.0, 0.0);
	vec3 squadColor = vec3(0.3, 0.6, 1.0);
	vec3 ambientLight = vec3(0.1, 0.1, 0.3);

    float diffuse = max(0.0, dot(normalize(n), lightDir));
	fColor = diffuse * squadColor + ambientLight;
}