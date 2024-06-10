
#include "uLight.h"
#include "GLutil.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

void uLight::makeFBO() {
	uNewFBO_FHCrD(buffers, shadowMapXRes, shadowMapYRes);
	uNewFBO_FHCrD(blurBuffers, shadowMapXRes, shadowMapYRes);
}

uLight::uLight(glm::vec3 v, glm::vec3 c, float i, float rad) {
	position = v;
	color = c;
	intensity = i;
	radius = rad;
}

uLight::uLight(float x, float y, float z, float r, float g, float b, float i, float rad) {
	position = glm::vec3(x, y, z);
	color = glm::vec3(r, g, b);
	intensity = i;
	radius = rad;
}

uLight::uLight(int XRES, int YRES, glm::vec3 v, glm::vec3 c, float i, float rad, glm::vec3 dir, float fov) {
	position = v;
	color = c;
	intensity = i;
	radius = rad;
	castsShadow = true;
	direction = glm::normalize(dir);
	shadowViewMatrix = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
	fovxy = fov;
	shadowProjMatrix = glm::perspective(fov, 1.0f, 0.01f, rad);
	shadowMapXRes = XRES;
	shadowMapYRes = YRES;
	shadowMapFinalXRes = XRES;
	shadowMapFinalYRes = YRES;
	zNear = 0.01f;
	zFar = rad;
	makeFBO();
}

uLight::uLight(int XRES, int YRES, float x, float y, float z, float r, float g, float b, float i, float rad, float dx, float dy, float dz, float fov) {
	position = glm::vec3(x, y, z);
	color = glm::vec3(r, g, b);
	intensity = i;
	radius = rad*0.9;
	direction = glm::normalize(glm::vec3(dx, dy, dz));
	castsShadow = true;
	shadowViewMatrix = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
	fovxy = fov;
	shadowProjMatrix = glm::perspective(fov, 1.0f, 0.01f, rad);
	shadowMapXRes = XRES;
	shadowMapYRes = YRES;
	shadowMapFinalXRes = XRES;
	shadowMapFinalYRes = YRES;
	zNear = 0.01f;
	zFar = rad;
	makeFBO();
}

void uLight::setShadowDimensions(int XRES, int YRES) {
	castsShadow = true;
	shadowMapXRes = std::fmax(64, std::fmin(shadowMapFinalXRes, XRES));
	shadowMapYRes = std::fmax(64, std::fmin(shadowMapFinalYRes, YRES));
}

void uLight::setFinalShadowDimensions(int XRES, int YRES) {
	castsShadow = true;
	shadowMapFinalXRes = XRES;
	shadowMapFinalYRes = YRES;
	makeFBO();
}

void uLight::setViewMatrix(glm::mat4 vmatrix) {
	shadowViewMatrix = vmatrix;
}