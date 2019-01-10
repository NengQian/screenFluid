#version 330 core

out vec3 fColor;

void main() {
	vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
	if (dot(circCoord, circCoord) > 1.0) {
	    discard;
	}
	fColor = vec3(0.0, 0.0, 1.0);
}
