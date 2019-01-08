#version 330 core

// vertex shader input
in vec3 aPosition;

void main() {
	// position in homogeneous coordinates
    gl_Position = vec4(aPosition, 1.0);
}
