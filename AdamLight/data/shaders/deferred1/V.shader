#version 120
attribute vec3 uStream0;
uniform mat4 modelViewProjMatrix;
uniform mat4 modelViewMatrix;
varying float Z;
void main() {
	gl_Position = modelViewProjMatrix*vec4(uStream0, 1.0);
	Z = (modelViewMatrix*vec4(uStream0, 1.0)).z;
}