#define _CRT_SECURE_NO_WARNINGS

#include "GLUtil.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdarg>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

void uSetUniformVertexAttrib(int program, const char* name, int x) {
	int loc = glGetAttribLocation(program, name);
	glLinkProgram(program);
}

void uSetUniform1i(int program, const char* name, int x) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniform1i(loc, x);
}

void uSetUniform1f(int program, const char* name, float x) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniform1f(loc, x);
}

void uSetUniform2f(int program, const char* name, float x, float y) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniform2f(loc, x, y);
}

void uSetUniform2f(int program, const char* name, glm::vec2 v) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniform2f(loc, v.x, v.y);
}

void uSetUniform3f(int program, const char* name, float x, float y, float z) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniform3f(loc, x, y, z);
}

void uSetUniform3f(int program, const char* name, glm::vec3 v) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void uSetUniform3fNoSwitch(const char* name, float x, float y, float z) {
	GLint prgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prgram);
	int loc = glGetUniformLocation(prgram, name);
	glUniform3f(loc, x, y, z);
}

void uSetUniform3fNoSwitch(const char* name, glm::vec3 v) {
	GLint prgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prgram);
	int loc = glGetUniformLocation(prgram, name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void uSetUniformMatrix3x3(int program, const char* name, glm::mat3 x) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &x[0][0]);
}

void uSetUniformMatrix4x4(int program, const char* name, glm::mat4 x) {
	glUseProgram(program);
	int loc = glGetUniformLocation(program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &x[0][0]);
}

void uSetUniform1iNoSwitch(const char* name, int x) {
	GLint prgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prgram);
	int loc = glGetUniformLocation(prgram, name);
	glUniform1i(loc, x);
}

void uSetUniform1fNoSwitch(const char* name, float x) {
	GLint prgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prgram);
	int loc = glGetUniformLocation(prgram, name);
	glUniform1f(loc, x);
}

void uSetUniformMatrix3x3NoSwitch(const char* name, glm::mat3 x) {
	GLint prgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prgram);
	int loc = glGetUniformLocation(prgram, name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &x[0][0]);
}

void uSetUniformMatrix4x4NoSwitch(const char* name, glm::mat4 x) {
	GLint prgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prgram);
	int loc = glGetUniformLocation(prgram, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &x[0][0]);
}

void uPrintMatrix(glm::mat4 m) {
	for (int x = 0; x < 4; x++) {
		std::cout << std::endl;
		for (int y = 0; y < 4; y++)
			std::cout << m[x][y] << " ";
	}
	std::cout << std::endl;
}

void uNewFBO_FCr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachment GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FHCr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, xres, yres, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachment GL_R16F (readable)\n" << std::endl;
}

void uNewFBO_FDr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint depth;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &depth);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, xres, yres, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	glDrawBuffer(GL_NONE);

	out[0] = fbuffer;
	out[2] = depth;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachment GL_DEPTH_COMPONENT16 (readable)\n" << std::endl;
}

void uNewFBO_FCrD(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenRenderbuffers(1, &depth);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, xres, yres);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH_COMPONENT (renderbuffer) and GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FHCrD(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenRenderbuffers(1, &depth);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, xres, yres, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, xres, yres);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH_COMPONENT (renderbuffer) and GL_R16F (readable)\n" << std::endl;
}

void uNewFBO_FHCrDr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, xres, yres, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, xres, yres, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = depth;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH_COMPONENT16 (readable) and GL_R16F (readable)\n" << std::endl;
}

void uNewFBO_FCrDr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, xres, yres, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = depth;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH_COMPONENT16 (readable) and GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FCrDSr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, xres, yres, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = depth;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH24_STENCIL8 (readable) and GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FCrCrDr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;
	GLuint normals;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);
	glGenTextures(1, &normals);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normals, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, xres, yres, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = normals;
	out[3] = depth;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH_COMPONENT16 (readable), GL_RGBA8 (readable), and GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FCrCrDSr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;
	GLuint normals;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);
	glGenTextures(1, &normals);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normals, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, xres, yres, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = normals;
	out[3] = depth;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH24_STENCIL8 (readable), GL_RGBA8 (readable), and GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FCrCrDrCr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;
	GLuint normals;
	GLuint light;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);
	glGenTextures(1, &normals);
	glGenTextures(1, &light);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, xres, yres, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normals, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, xres, yres, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	glBindTexture(GL_TEXTURE_2D, light);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, light, 0);

	GLenum DrawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(3, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = normals;
	out[3] = depth;
	out[4] = light;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH_COMPONENT16 (readable), GL_RGBA8 (readable), GL_RGBA8 (readable), and GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FCrCrDrCrCr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;
	GLuint normals;
	GLuint light;
	GLuint data;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);
	glGenTextures(1, &normals);
	glGenTextures(1, &light);
	glGenTextures(1, &data);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, xres, yres, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normals, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, xres, yres, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	glBindTexture(GL_TEXTURE_2D, light);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, xres, yres, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, light, 0);

	glBindTexture(GL_TEXTURE_2D, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, data, 0);

	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
	glDrawBuffers(4, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = normals;
	out[3] = depth;
	out[4] = light;
	out[5] = data;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH_COMPONENT16 (readable), GL_RGBA8 (readable), GL_RGBA8 (readable), and GL_RGBA8 (readable)\n" << std::endl;
}

void uNewFBO_FCrCrDSrCr(GLuint out[], unsigned const int &xres, unsigned const int &yres) {
	GLuint fbuffer;
	GLuint color;
	GLuint depth;
	GLuint normals;
	GLuint light;

	glGenFramebuffers(1, &fbuffer);
	glGenTextures(1, &color);
	glGenTextures(1, &depth);
	glGenTextures(1, &normals);
	glGenTextures(1, &light);

	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

	glBindTexture(GL_TEXTURE_2D, normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, xres, yres, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normals, 0);

	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, xres, yres, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	glBindTexture(GL_TEXTURE_2D, light);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, light, 0);

	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	out[0] = fbuffer;
	out[1] = color;
	out[2] = normals;
	out[3] = depth;
	out[4] = light;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error! Framebuffer not properly created!\n\n");
	else
		std::cout << "Framebuffer created successfully with dimensions " << xres << "x" << yres << " and attachments GL_DEPTH24_STENCIL8 (readable), GL_RGBA8 (readable), GL_RGBA8 (readable), and GL_RGBA8 (readable)\n" << std::endl;
}

void uRenderToScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void uRectOnScreenFFP() {
	//DON'T USE THIS IN YOUR CODE THIS IS NOT GOOD//
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(0, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0);
	glVertex2f(1.0f, -1.0f);
	glEnd();
}

void uRectOnScreenFFPWriteDepth() {
	//DON'T USE THIS IN YOUR CODE THIS IS NOT GOOD//
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(0, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0);
	glVertex2f(1.0f, -1.0f);
	glEnd();
}

void uRectOnScreenFFP(float rsX, float rsY) {
	//DON'T USE THIS IN YOUR CODE THIS IS NOT GOOD//
	float a = 1920.0f / rsX;
	float b = 1080.0f / rsY;
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(0, b);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(a, b);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(a, 0);
	glVertex2f(1.0f, -1.0f);
	glEnd();
}

void uDrawCubeFFP(float x, float y, float z) {
	//DON'T USE THIS IN YOUR CODE THIS IS NOT GOOD//
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(x + 0.0f, y + 0.0f, z + 0.0f); glTexCoord2f(0, 0);
	glVertex3f(x + 1.0f, y + 0.0f, z + 0.0f); glTexCoord2f(1, 0);
	glVertex3f(x + 1.0f, y + 1.0f, z + 0.0f); glTexCoord2f(1, 1);
	glVertex3f(x + 0.0f, y + 1.0f, z + 0.0f); glTexCoord2f(0, 1);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(x + 0.0f, y + 0.0f, z + -1.0f); glTexCoord2f(0, 0);
	glVertex3f(x + 1.0f, y + 0.0f, z + -1.0f); glTexCoord2f(1, 0);
	glVertex3f(x + 1.0f, y + 1.0f, z + -1.0f); glTexCoord2f(1, 1);
	glVertex3f(x + 0.0f, y + 1.0f, z + -1.0f); glTexCoord2f(0, 1);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x + 1.0f, y + 0.0f, z + 0.0f); glTexCoord2f(0, 0);
	glVertex3f(x + 1.0f, y + 0.0f, z + -1.0f); glTexCoord2f(1, 0);
	glVertex3f(x + 1.0f, y + 1.0f, z + -1.0f); glTexCoord2f(1, 1);
	glVertex3f(x + 1.0f, y + 1.0f, z + 0.0f); glTexCoord2f(0, 1);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(x + 0.0f, y + 0.0f, z + 0.0f); glTexCoord2f(0, 0);
	glVertex3f(x + 0.0f, y + 0.0f, z + -1.0f); glTexCoord2f(1, 0);
	glVertex3f(x + 0.0f, y + 1.0f, z + -1.0f); glTexCoord2f(1, 1);
	glVertex3f(x + 0.0f, y + 1.0f, z + 0.0f); glTexCoord2f(0, 1);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x + 0.0f, y + 1.0f, z + 0.0f); glTexCoord2f(0, 0);
	glVertex3f(x + 1.0f, y + 1.0f, z + 0.0f); glTexCoord2f(1, 0);
	glVertex3f(x + 1.0f, y + 1.0f, z + -1.0f); glTexCoord2f(1, 1);
	glVertex3f(x + 0.0f, y + 1.0f, z + -1.0f); glTexCoord2f(0, 1);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(x + 0.0f, y + 0.0f, z + 0.0f); glTexCoord2f(0, 0);
	glVertex3f(x + 1.0f, y + 0.0f, z + 0.0f); glTexCoord2f(1, 0);
	glVertex3f(x + 1.0f, y + 0.0f, z + -1.0f); glTexCoord2f(1, 1);
	glVertex3f(x + 0.0f, y + 0.0f, z + -1.0f); glTexCoord2f(0, 1);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glDisable(GL_TEXTURE_2D);
	glEnd();
}

void uDrawOctahedronFFP(const float &x, const float &y, const float &z, const float &s) {
	glBegin(GL_TRIANGLES);
	glVertex3f(0 + x, s + y, 0 + z);
	glVertex3f(s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, s + z);

	glVertex3f(0 + x, s + y, 0 + z);
	glVertex3f(-s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, s + z);

	glVertex3f(0 + x, -s + y, 0 + z);
	glVertex3f(s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, s + z);

	glVertex3f(0 + x, -s + y, 0 + z);
	glVertex3f(-s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, s + z);

	glVertex3f(0 + x, s + y, 0 + z);
	glVertex3f(s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, -s + z);

	glVertex3f(0 + x, s + y, 0 + z);
	glVertex3f(-s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, -s + z);

	glVertex3f(0 + x, -s + y, 0 + z);
	glVertex3f(s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, -s + z);

	glVertex3f(0 + x, -s + y, 0 + z);
	glVertex3f(-s + x, 0 + y, 0 + z);
	glVertex3f(0 + x, 0 + y, -s + z);
	glEnd();
}

GLuint uLoadBMP(const char* imagepath) {
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char * data;
	std::cout << "Loading BMP texture from " << imagepath << "..." << std::endl;
	FILE * file = fopen(imagepath, "rb");
	if(!file) { 
		fprintf(stderr, "Opening Image failed!\n");
		return -1;
	}
	if(fread(header, 1, 54, file) != 54) {
		fprintf(stderr, "Not a 24-bit BMP file!\n");
		return -1;
	}
	if(header[0] != 'B' || header[1] != 'M') {
		fprintf(stderr, "Not a 24-bit BMP file!\n");
		return -1;
	}
	dataPos = *(int*) &(header[0x0A]);
	imageSize = *(int*) &(header[0x22]);
	width = *(int*) &(header[0x12]);
	height = *(int*) &(header[0x16]);
	if (imageSize == 0) imageSize = width*height*3;
	if (dataPos == 0) dataPos = 54;
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

	glGenerateMipmap(GL_TEXTURE_2D);

	std::cout << "Loaded texture successfully!\n" << std::endl;

	return textureID;
}

std::string uReadFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint uLoadShader(const char* vertex_path, const char* fragment_path) {
	std::cout << "Loading Vertex & Fragment shaders from " << vertex_path << " & " << fragment_path << std::endl;

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::string vertShaderStr = uReadFile(vertex_path);
	std::string fragShaderStr = uReadFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint result = GL_FALSE;
	int logLength;

	std::cout << "Compiling vertex shader...";
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	std::string vertShaderError;
	vertShaderError.resize(logLength);
	glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
	std::cout << vertShaderError << std::endl;

	std::cout << "Compiling fragment shader...";
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);


	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

	std::string fragShaderError;
	fragShaderError.resize(logLength);
	glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
	std::cout << fragShaderError << std::endl;

	std::cout << "Linking program...";
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	for (int i = 0; i < 16; i++) {
		std::ostringstream oss;
		oss << "uStream" << i;
		std::string s = oss.str();
		glBindAttribLocation(program, i, s.c_str());
	}
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::string programError;
	programError.resize(logLength);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << programError << std::endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	glUseProgram(program);
	uSetUniform1i(program, "TA", 0);
	uSetUniform1i(program, "TB", 1);
	uSetUniform1i(program, "TC", 2);
	uSetUniform1i(program, "TD", 3);
	uSetUniform1i(program, "TE", 4);
	uSetUniform1i(program, "TF", 5);
	uSetUniform1i(program, "TG", 6);
	uSetUniform1i(program, "TH", 7);
	uSetUniform1i(program, "TI", 8);
	uSetUniform1i(program, "TJ", 9);
	uSetUniform1i(program, "TK", 10);
	uSetUniform1i(program, "TL", 11);
	uSetUniform1i(program, "TM", 12);
	uSetUniform1i(program, "TN", 13);
	uSetUniform1i(program, "TO", 14);
	uSetUniform1i(program, "TP", 15);
	uSetUniform1i(program, "TQ", 16);

	std::cout << "Shaders loaded successfully!\n" << std::endl;

	return program;
}

void uRenderToFBO(int FBOID) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
}

void uPrintCurrentProgram() {
	GLint prgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prgram);
	std::cout << "Current Program: " << prgram << std::endl;
}

void uFilterDepthWrite(int FBOID, int tex) {
	uRenderToFBO(FBOID);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, tex);
	uRectOnScreenFFPWriteDepth();
}

void uFilterFBOToFBO(int FBOID, int shader, int xres, int yres, int texturesToUse, ...) {
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	uRenderToFBO(FBOID);
	glViewport(0, 0, xres, yres);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader);

	va_list arguments;
	va_start(arguments, texturesToUse);
	for (int x = 0; x < texturesToUse; x++) {
		glActiveTexture(GL_TEXTURE0 + x);
		glBindTexture(GL_TEXTURE_2D, va_arg(arguments, GLuint));
	}
	va_end(arguments);

	uRectOnScreenFFP();

	glActiveTexture(GL_TEXTURE0);
}

void uFilterFBOToFBONoClear(int FBOID, int shader, int xres, int yres, int texturesToUse, ...) {
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	uRenderToFBO(FBOID);
	glViewport(0, 0, xres, yres);
	glUseProgram(shader);

	va_list arguments;
	va_start(arguments, texturesToUse);
	for (int x = 0; x < texturesToUse; x++) {
		glActiveTexture(GL_TEXTURE0 + x - 1);
		glBindTexture(GL_TEXTURE_2D, va_arg(arguments, GLuint));
	}
	va_end(arguments);

	uRectOnScreenFFP();

	glActiveTexture(GL_TEXTURE0);
}

void uFilterFBOToFBONoClear(int FBOID, int shader, int x0, int y0, int x1, int y1, int texturesToUse, ...) {
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	uRenderToFBO(FBOID);
	glViewport(x0, y0, x1, y1);
	glUseProgram(shader);

	va_list arguments;
	va_start(arguments, texturesToUse);
	for(int x = 0; x < texturesToUse; x++) {
		glActiveTexture(GL_TEXTURE0 + x - 1);
		glBindTexture(GL_TEXTURE_2D, va_arg(arguments, GLuint));
	}
	va_end(arguments);

	uRectOnScreenFFP();

	glActiveTexture(GL_TEXTURE0);
}

glm::vec3 uRoundVecTo(glm::vec3 v, float divisionNum) {
	v.x = round(v.x / divisionNum) * divisionNum;
	v.y = round(v.y / divisionNum) * divisionNum;
	v.z = round(v.z / divisionNum) * divisionNum;
	return v;
}

glm::vec3 uMul3x4(glm::vec3 p, glm::mat4 matrix) {
	return glm::vec3(glm::vec4(p, 1.0) * matrix);
}

glm::mat4 uRotateToPoint(glm::vec3 O, glm::vec3 P, glm::vec3 U) {
	glm::vec3 D = (O - P);
	glm::vec3 right = glm::cross(U, D);
	right = glm::normalize(right);
	glm::vec3 backwards = glm::cross(right, U);
	backwards = glm::normalize(backwards);
	glm::vec3 up = glm::cross(backwards, right);
	glm::mat4 rot = glm::mat4(right.x, right.y, right.z, 0, up.x, up.y, up.z, 0, backwards.x, backwards.y, backwards.z, 0, 0, 0, 0, 1);
	return rot;
}