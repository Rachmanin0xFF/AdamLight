
#ifndef TESTCAMERA_H
#define TESTCAMERA_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TestCamera {
private:
	glm::mat4x4 viewMatrix;
	glm::mat4x4 projMatrix;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 viewDirection;
	glm::vec3 upVec;
	float xRot;
	float yRot;
	double pmouseX;
	double pmouseY;
	bool mouseGrab;
	bool wasTab;
public:
	TestCamera();
	float zNear = 0.1f;
	float zFar = 10000000.0f;
	void tick(GLFWwindow* w, float xres, float yres, float speedMult);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix();
	glm::vec3 getPos();
	glm::vec3 getDirection();
	glm::vec3 getUp();
};

#endif