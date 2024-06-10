
#ifndef ULIGHT_H
#define ULIGHT_H

#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class uLight {
private:
	void makeFBO();
public:
	glm::vec3 position;
	glm::vec3 color;
	float intensity = 0.0f;
	float radius = 0.0f;
	int shadowMapXRes;
	int shadowMapYRes;
	int shadowMapFinalXRes;
	int shadowMapFinalYRes;

	bool castsShadow = false;
	glm::vec3 direction;
	float fovxy = 0.0f;
	GLuint buffers[3];
	GLuint blurBuffers[3];
	glm::mat4 shadowViewMatrix;
	glm::mat4 shadowProjMatrix;
	float zNear;
	float zFar;

	uLight(glm::vec3 v, glm::vec3 c, float i, float rad);
	uLight(float x, float y, float z, float r, float g, float b, float i, float rad);

	uLight(int XRES, int YRES, glm::vec3 v, glm::vec3 c, float i, float rad, glm::vec3 dir, float fov);
	uLight(int XRES, int YRES, float x, float y, float z, float r, float g, float b, float i, float rad, float dx, float dy, float dz, float fov);

	void setShadowDimensions(int XRES, int YRES);
	void setFinalShadowDimensions(int XRES, int YRES);

	void setViewMatrix(glm::mat4 vmatrix);
};

#endif