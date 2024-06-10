#define GLM_FORCE_RADIANS

#include "TestCamera.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

TestCamera::TestCamera() {
	position = glm::vec3(0, 0, -5);
	projMatrix = glm::perspectiveFov(3.1415f / 4.0f, 1024.0f / 640.0f, 1.0f, zNear, zFar);
	mouseGrab = false;
}

void TestCamera::tick(GLFWwindow* w, float xres, float yres, float deltaTime) {
	float speedMult = deltaTime*60.f;
	if (glfwGetKey(w, GLFW_KEY_TAB) == GLFW_PRESS && !wasTab) {
		mouseGrab = !mouseGrab;
		wasTab = true;
	}
	if (glfwGetKey(w, GLFW_KEY_TAB) == GLFW_RELEASE)
		wasTab = false;
	float speed = 0.03f*speedMult*0.3f;
	if (glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS)
		speed = 0.1f*speedMult*0.3f;
	if (glfwGetKey(w, GLFW_KEY_R) == GLFW_PRESS)
		speed = 0.0025f*speedMult*0.3f;
	viewMatrix = glm::mat4();
	projMatrix = glm::mat4();
	if (mouseGrab)
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	double mouseX = 0;
	double mouseY = 0;
	glfwGetCursorPos(w, &mouseX, &mouseY);
	if(mouseGrab)
		glfwSetCursorPos(w, xres/2, yres/2);

	float deltaMouseX = static_cast<float>(pmouseX - mouseX);
	float deltaMouseY = static_cast<float>(pmouseY - mouseY);
	if (!mouseGrab) {
		deltaMouseX = 0;
		deltaMouseY = 0;
	}
	xRot += deltaMouseY / 512.0f;
	yRot += deltaMouseX / 512.0f;
	if (xRot > 3.141592/2.0)
		xRot = 3.141592/2.0;
	if (xRot < -3.141592/2.0)
		xRot = -3.141592/2.0;

	glm::vec3 direction(cos(xRot) * sin(yRot), sin(xRot), cos(xRot) * cos(yRot));
	glm::vec3 right = glm::vec3(sin(yRot - 3.14159f / 2.0f), 0, cos(yRot - 3.14159f / 2.0f));
	glm::vec3 up = glm::cross(right, direction);
	upVec = up;

	direction = glm::normalize(direction);
	right = glm::normalize(right);
	up = glm::normalize(up);
	
	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
		velocity += direction * speed;
	}
	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
		velocity -= direction * speed;
	}
	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
		velocity += right * speed;
	}
	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS ){
		velocity -= right * speed;
	}
	if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS) {
		velocity += glm::vec3(0, speed, 0);
	}
	if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		velocity -= glm::vec3(0, speed, 0);
	}
	position += velocity;
	velocity -= velocity*deltaTime*7.0f;

	viewMatrix = glm::lookAt(position, position + direction, up);
	viewDirection = direction;

	projMatrix = glm::perspectiveFov(3.1415f / 4.0f, xres/yres, 1.0f, zNear, zFar);

	glfwGetCursorPos(w, &pmouseX, &pmouseY);
}

glm::mat4 TestCamera::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 TestCamera::getProjMatrix() {
	return projMatrix;
}

glm::vec3 TestCamera::getPos() {
	return position;
}

glm::vec3 TestCamera::getUp() {
	return upVec;
}

glm::vec3 TestCamera::getDirection() {
	return viewDirection;
}