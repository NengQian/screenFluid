#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform vec2 uScreenSize;
uniform float uSpriteSize;

out vec4 vEyeSpacePosition;
//out vec4 vs_color;
void main() {
    vec4 p = uModelViewMatrix * vec4(aPosition, 1.0);
    vEyeSpacePosition = p;

    vec4 projVoxel = uProjectionMatrix * vec4(uSpriteSize, uSpriteSize, p.z, p.w);
    vec2 projSize = uScreenSize * projVoxel.xy / projVoxel.w;   // here we just want to calculate the sprite size at p.z this distance... So if the z bigger, the sprite smaller.
    
    gl_PointSize = 0.25 * (projSize.x+projSize.y);
    gl_Position = uProjectionMatrix * p;

// debug
    // const vec4 colors[] = vec4[3](vec4(1.0, 1.0, 1.0, 1.0),
	// 							vec4(0.5, 0.5, 0.5, 1.0),
	// 							vec4(0.0, 0.0, 0.0, 1.0));
    // int test = int(floor(gl_VertexID/3.0));
    // int index = gl_VertexID - (3*(test));
    // vs_color = colors[index];

}
