#version 330 core

attribute vec3 uStream0;
uniform mat4 modelViewProjMatrix;
void main() {
	gl_Position = modelViewProjMatrix*vec4(uStream0, 1.0);
}