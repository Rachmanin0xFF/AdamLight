#version 330 core

attribute vec3 uStream0; //Positions
attribute vec3 uStream2; //Normals
attribute vec3 uStream8; //Texture coordinates
attribute vec3 uStream9; //Tangents
attribute vec3 uStream10; //Bitangents
uniform mat4 modelViewProjMatrix;
uniform mat3 normalMatrix;
uniform mat3 modelViewMatrix;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec2 texCoord;
void main() {
	gl_Position = modelViewProjMatrix*vec4(uStream0, 1.0);
	normal = normalMatrix * normalize(uStream2);
	texCoord = uStream8.st;
	tangent = normalMatrix * normalize(uStream9.xyz);
	bitangent = normalMatrix * normalize(uStream10.xyz);
}