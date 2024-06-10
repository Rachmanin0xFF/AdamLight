
//--------------------------------------------------------------------//

#include <GL/glew.h>  

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include <vector>

#include <time.h>
#include <math.h>

#include "GLUtil.h"
#include "FPSCounter.h"
#include "TestCamera.h"
#include "uLight.h"
#include "uMesh.h"
#include "uModel.h"

//--------------------------------------------------------------------//
GLFWwindow* window;
std::vector<uLight> lightList;

int resSX = 1024;
int resSY = 640;
int prsx = 1024;
int prsy = 640;

FPSCounter FrameSpeedCounter;
GLuint gBuffer[6];
uMesh lightR1Mesh;
uMesh coneR1Mesh;

int shadowBlurs = 3;

GLuint SSBuffer0[3];
GLuint SSBuffer1[3];
GLuint SSBuffer2[3];

GLuint firstPass[2];
GLuint brightSpots[2];
GLuint blurredBrightA[2];
GLuint blurredBrightB[2];
int bloomDiv = 32;

GLuint sdWriteGBuffer;
GLuint sdPlain;
GLuint sdEdges;
GLuint sdDispDepth;
GLuint sdSSAO;
GLuint sdBokeh;
GLuint sdDispSine;
GLuint sdDeferred1;
GLuint sdDeferred2;
GLuint sdDeferred2S;
GLuint sdShadowVBlur;
GLuint sdShadowHBlur;
GLuint sdBlueChroma;
GLuint sdProcess;
GLuint sdGamma;
GLuint sdSSR;
GLuint sdGetBright;
GLuint sdBloomBlur;

GLuint tex;
GLuint tex2;
GLuint tex1;
GLuint tex4;
GLuint randomTex;
GLuint blueNoiseTex;

TestCamera eyeCam;

glm::mat4x4 projectionMatrix;
glm::mat4x4 viewMatrix;
glm::mat4x4 modelMatrix;

uMesh testMesh;
uMesh lattice;
uModel testModel;
uModel testModel2;

int deepTick = 0;
//--------------------------------------------------------------------//
void renderStep();
void stencilStep(int i);
void loop();
void renderGL(int mode);
void setUpGL();
bool shouldClose();
void killWindow();
//--------------------------------------------------------------------//

void deleteFBOS() {
	glDeleteFramebuffers(1, &gBuffer[0]);
	glDeleteFramebuffers(1, &SSBuffer0[0]);
	glDeleteFramebuffers(1, &SSBuffer1[0]);
	glDeleteFramebuffers(1, &SSBuffer2[0]);
}

void remakeFBOS() {
	deleteFBOS();
	uNewFBO_FCrCrDrCrCr(gBuffer, resSX, resSY);
	uNewFBO_FCrDr(SSBuffer0, resSX, resSY);
	uNewFBO_FCrDr(SSBuffer1, resSX, resSY);
	uNewFBO_FCrDr(SSBuffer2, resSX, resSY);

	uNewFBO_FCr(firstPass, resSX, resSY);
	uNewFBO_FCr(brightSpots, resSX / bloomDiv, resSY / bloomDiv);
	//uNewFBO_FCrDr(blurredBrightA, resSX / bloomDiv / 8, resSY / bloomDiv / 8);
	//uNewFBO_FCr(blurredBrightB, resSX / bloomDiv / 64, resSY / bloomDiv / 64);
}

void initialize() {
	fprintf(stdout, "/////CORE INITIALIZATION STARTED/////\n");

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	else
		fprintf(stdout, "GLFW initialized successfully!\n");

	window = glfwCreateWindow(1280, 720, "AutumnLight", NULL, NULL);
	//window = glfwCreateWindow(1920, 1080, "AutumnLight", glfwGetPrimaryMonitor(), NULL);
	//window = glfwCreateWindow(1280, 720, "AutumnLight", glfwGetPrimaryMonitor(), NULL);
	glfwGetWindowSize(window, &resSX, &resSY);
	
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else
		fprintf(stdout, "GLFW window created successfully!.\n");
	glfwMakeContextCurrent(window);

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	else
		fprintf(stdout, "GLEW initialized successfully!\n");
	fprintf(stdout, "/////CORE INITIALIZATION COMPLETED/////\n\n");

	//--------------------------------------------------------------------//

	srand(time(NULL));

	setUpGL();

	projectionMatrix = glm::mat4();
	viewMatrix = glm::mat4();
	modelMatrix = glm::mat4();

	lightR1Mesh.loadOBJRAW("data/light/icosahedronlight.obj", true);
	coneR1Mesh.loadOBJRAW("data/light/conelight.obj", true);
	sdPlain = uLoadShader("data/shaders/plain/V.shader", "data/shaders/plain/F.shader");
	sdWriteGBuffer = uLoadShader("data/shaders/writeGBuffer/V.shader", "data/shaders/writeGBuffer/F.shader");
	sdEdges = uLoadShader("data/shaders/edges/V.shader", "data/shaders/edges/F.shader");
	sdDispDepth = uLoadShader("data/shaders/dispDepth/V.shader", "data/shaders/dispDepth/F.shader");
	sdSSAO = uLoadShader("data/shaders/SSAO/V.shader", "data/shaders/SSAO/F.shader");
	sdBokeh = uLoadShader("data/shaders/bokeh/V.shader", "data/shaders/bokeh/F.shader");
	sdDispSine = uLoadShader("data/shaders/dispSine/V.shader", "data/shaders/dispSine/F.shader");
	sdDeferred1 = uLoadShader("data/shaders/deferred1/V.shader", "data/shaders/deferred1/F.shader");
	sdDeferred2 = uLoadShader("data/shaders/deferred2/V.shader", "data/shaders/deferred2/F.shader");
	sdDeferred2S = uLoadShader("data/shaders/deferred2Shadow/V.shader", "data/shaders/deferred2Shadow/F.shader");
	sdShadowVBlur = uLoadShader("data/shaders/shadowBlur/shadowVBlur/V.shader", "data/shaders/shadowBlur/shadowVBlur/F.shader");
	sdShadowHBlur = uLoadShader("data/shaders/shadowBlur/shadowHBlur/V.shader", "data/shaders/shadowBlur/shadowHBlur/F.shader");
	sdBlueChroma = uLoadShader("data/shaders/blueChroma/V.shader", "data/shaders/blueChroma/F.shader");
	sdProcess = uLoadShader("data/shaders/process/V.shader", "data/shaders/process/F.shader");
	sdGamma = uLoadShader("data/shaders/gamma/V.shader", "data/shaders/gamma/F.shader");
	sdSSR = uLoadShader("data/shaders/SSR/V.shader", "data/shaders/SSR/F.shader");
	sdGetBright = uLoadShader("data/shaders/getBright/V.shader", "data/shaders/getBright/F.shader");
	sdBloomBlur = uLoadShader("data/shaders/bloomBlur/V.shader", "data/shaders/bloomBlur/F.shader");

	randomTex = uLoadBMP("data/textures/random.bmp");
	blueNoiseTex = uLoadBMP("data/textures/blueN.bmp");
	
	testModel.loadModelOBJ("data/test_models/neatshape.obj");

	lightList.push_back(uLight(1024, 1024, glm::vec3(0, 5, 0), glm::vec3(1.f, 1.f, 1.f), 1.6f, 60, glm::vec3(0.5f, -1, 0.1f), 40));
	lightList.push_back(uLight(1024, 1024, glm::vec3(0, 5, 0), glm::vec3(1.f, 0.8f, 0.2f), 2.6f, 60, glm::vec3(0.5f, -1, 0.1f), 40));
	lightList.push_back(uLight(1024, 1024, glm::vec3(0, 5, 0), glm::vec3(0.2f, 0.4f, 1.f), 2.6f, 60, glm::vec3(0.5f, -1, 0.1f), 40));
	lightList.push_back(uLight(glm::vec3(0, 1000, 1000), glm::vec3(1, 1, 1), 0.2f, 10000.0f));

	//lightList.push_back(uLight(glm::vec3(0, 0, 0), glm::vec3(1.0, 0.56, 0.06), 0.5f, 10.0f));
	//lightList.push_back(uLight(glm::vec3(0, 0, 0), glm::vec3(0.02, 0.03, 1.0), 0.5f, 10.0f));
	//lightList.push_back(uLight(glm::vec3(0, 0, 0), glm::vec3(0.095, 1, 0.1), 0.5f, 10.0f));
	//lightList.push_back(uLight(glm::vec3(0, 0, 0), glm::vec3(1, 0.34, 0.44), 0.5f, 10.0f));
	
	//--------------------------------------------------------------------//
	remakeFBOS();
	prsx = resSX;
	prsy = resSY;
	std::cout << "Setup over. System online." << std::endl;
	std::cout << GL_ZERO << std::endl;
	std::cout << GL_ONE << std::endl;
}

void setSphere(int i, glm::mat4 lightingmatrix) {
	if (!lightList[i].castsShadow) {
		GLuint shaderP = sdDeferred2;
		
		lightingmatrix = glm::scale(lightingmatrix, glm::vec3(lightList[i].radius));
		glUseProgram(shaderP);
		uSetUniformMatrix4x4(shaderP, "modelViewProjMatrix", projectionMatrix * viewMatrix * lightingmatrix);
		uSetUniform3f(shaderP, "lPos", lightList[i].position);
		uSetUniform1f(shaderP, "lRadius", lightList[i].radius);
		uSetUniform1f(shaderP, "lIntensity", lightList[i].intensity);
		uSetUniform3f(shaderP, "lColor", lightList[i].color);

		lightR1Mesh.display();
	}
	else {
		GLuint shaderP = sdDeferred2S;
		
		float xzlength = sqrt(lightList[i].direction.x * lightList[i].direction.x + lightList[i].direction.z * lightList[i].direction.z);
		float xRot = atan2(lightList[i].direction.y, xzlength);
		float yRot = atan2(lightList[i].direction.x, lightList[i].direction.z);
		glm::mat4 coneMatrix;
		coneMatrix = glm::rotate(coneMatrix, yRot * 57.2957795f, glm::vec3(0, 1, 0));
		coneMatrix = glm::rotate(coneMatrix, -xRot * 57.2957795f, glm::vec3(1, 0, 0));
		
		float scalexy = tan(lightList[i].fovxy * 0.0174532925f) * lightList[i].radius / 2.0f;
		coneMatrix = glm::scale(coneMatrix, glm::vec3(scalexy, scalexy, lightList[i].radius));
		
		uSetUniformMatrix4x4(shaderP, "modelViewProjMatrix", projectionMatrix * viewMatrix * lightingmatrix * coneMatrix);
		uSetUniform2f(shaderP, "SHADOWRES", lightList[i].shadowMapXRes, lightList[i].shadowMapYRes);
		uSetUniform2f(shaderP, "shadowResMult", static_cast<float>(lightList[i].shadowMapFinalXRes) / static_cast<float>(lightList[i].shadowMapXRes), static_cast<float>(lightList[i].shadowMapFinalYRes) / static_cast<float>(lightList[i].shadowMapYRes));
		uSetUniform3f(shaderP, "lPos", lightList[i].position);
		uSetUniform1f(shaderP, "time", deepTick);
		uSetUniform1f(shaderP, "lRadius", lightList[i].radius);
		uSetUniform1f(shaderP, "shadowZNear", lightList[i].zNear);
		uSetUniform1f(shaderP, "shadowZFar", lightList[i].zFar);
		uSetUniform1f(shaderP, "lIntensity", lightList[i].intensity);
		uSetUniform3f(shaderP, "lColor", lightList[i].color);
		uSetUniform3f(shaderP, "lDirection", lightList[i].direction);
		uSetUniform3f(shaderP, "shadowViewDir", uMul3x4(lightList[i].direction, glm::inverse(viewMatrix)));
		uSetUniformMatrix4x4(shaderP, "shadowViewProjMatrix", lightList[i].shadowProjMatrix * lightList[i].shadowViewMatrix);
		uSetUniformMatrix4x4(shaderP, "shadowViewMatrix", lightList[i].shadowViewMatrix);
		uSetUniformMatrix4x4(shaderP, "eyeViewToShadowViewProjMatrix", lightList[i].shadowProjMatrix * lightList[i].shadowViewMatrix * glm::inverse(viewMatrix));
		uSetUniformMatrix4x4(shaderP, "eyeViewToShadowViewMatrix", lightList[i].shadowViewMatrix * glm::inverse(viewMatrix));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, lightList[i].buffers[1]);
		glActiveTexture(GL_TEXTURE0);
		
		glUseProgram(shaderP);

		coneR1Mesh.display();
	}
}

// Taken from stackoverflow, shortest distance between line segment wv and p.
float minimum_distance(glm::vec2 v, glm::vec2 w, glm::vec2 p) {
	const float l2 = glm::distance2(v, w);
	if (l2 == 0.0) return glm::distance(p, v);
	const float t = glm::dot(p - v, w - v) / l2;
	if (t < 0.0) return glm::distance(p, v);
	else if (t > 1.0) return glm::distance(p, w);
	const glm::vec2 projection = v + t * (w - v);
	return glm::distance(p, projection);
}

void loop() {
	glfwSetWindowTitle(window, std::to_string(FrameSpeedCounter.FPS).c_str());
	uRenderToFBO(gBuffer[0]);
	glViewport(0, 0, resSX, resSY);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < lightList.size(); i++) {
		if (glfwGetKey(window, GLFW_KEY_1 + i) != GLFW_RELEASE) {
			lightList[i].setViewMatrix(eyeCam.getViewMatrix());
			lightList[i].position = eyeCam.getPos();
			lightList[i].direction = eyeCam.getDirection();
		}
	}

	glfwGetWindowSize(window, &resSX, &resSY);
	if (prsx != resSX || prsy != resSY) {
		remakeFBOS();
		prsx = resSX;
		prsy = resSY;
	}
	glViewport(0, 0, resSX, resSY);
	
	eyeCam.tick(window, static_cast<float>(resSX), static_cast<float>(resSY), FrameSpeedCounter.deltaTime);
	FrameSpeedCounter.tick();
	//--------------------------------------------------------------------//

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
	viewMatrix = eyeCam.getViewMatrix();
	projectionMatrix = eyeCam.getProjMatrix();
	glUseProgram(sdDeferred1);
	for (int i = 0; i < lightList.size(); i++) {
		if (lightList[i].castsShadow) {
			//--------------------------------------------------------------------//

			glm::vec3 view_dir = glm::vec3(eyeCam.getDirection());
			glm::vec3 eye_to_light = glm::vec3(lightList[i].position - eyeCam.getPos());
			glm::vec3 n = eye_to_light - lightList[i].radius*glm::normalize(eye_to_light);

			if (lightList[i].position == eyeCam.getPos() || glm::dot(n, view_dir) > 0. || glm::dot(eye_to_light, view_dir) > 0.) {
				//--------------------------------------------------------------------//
				glm::vec3 r = glm::vec3(lightList[i].direction);
				glm::vec3 u = glm::vec3(0, -1, 0);
				float angle = glm::angle(u, r);
				glm::vec3 q = glm::rotate(glm::vec3(eyeCam.getPos() - lightList[i].position), angle, glm::cross(r, u));

				glm::vec2 p = glm::vec2(glm::length(glm::vec2(q.x, q.z)), q.y);
				float w = lightList[i].radius*tan(lightList[i].fovxy / 2. * 180 / 3.141592);
				glm::vec2 c0 = glm::vec2(0, 0);
				glm::vec2 c1 = glm::vec2(-w, -lightList[i].radius);
				glm::vec2 c2 = glm::vec2(w, -lightList[i].radius);

				float dist = fmin(minimum_distance(c0, c1, p), fmin(minimum_distance(c0, c2, p), minimum_distance(c1, c2, p)));
				if (p.y > -lightList[i].radius && p.x < w*(p.y / lightList[i].radius) && p.x > -w / (p.y / lightList[i].radius)) dist = 0.;
				//--------------------------------------------------------------------//

				float distTo = fmax(0.00001, dist) / 4.f / fmax(1., static_cast<float>(shadowBlurs) - 0.5);
				lightList[i].setShadowDimensions(static_cast<int>(static_cast<float>(lightList[i].shadowMapFinalXRes) / distTo), static_cast<int>(static_cast<float>(lightList[i].shadowMapFinalYRes) / distTo));
				uRenderToFBO(lightList[i].buffers[0]);
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				viewMatrix = lightList[i].shadowViewMatrix;
				projectionMatrix = lightList[i].shadowProjMatrix;
				glViewport(0, 0, lightList[i].shadowMapXRes, lightList[i].shadowMapYRes);
				uSetUniform1f(sdDeferred1, "zNear", lightList[i].zNear); uSetUniform1f(sdDeferred1, "zFar", lightList[i].zFar);
				renderGL(1);

				glDepthMask(GL_FALSE);
				uSetUniform2f(sdShadowVBlur, "RES", lightList[i].shadowMapXRes, lightList[i].shadowMapYRes);
				uSetUniform2f(sdShadowHBlur, "RES", lightList[i].shadowMapXRes, lightList[i].shadowMapYRes);
				uSetUniform2f(sdShadowVBlur, "shadowResMult", static_cast<float>(lightList[i].shadowMapFinalXRes) / static_cast<float>(lightList[i].shadowMapXRes), static_cast<float>(lightList[i].shadowMapFinalYRes) / static_cast<float>(lightList[i].shadowMapYRes));
				uSetUniform2f(sdShadowHBlur, "shadowResMult", static_cast<float>(lightList[i].shadowMapFinalXRes) / static_cast<float>(lightList[i].shadowMapXRes), static_cast<float>(lightList[i].shadowMapFinalYRes) / static_cast<float>(lightList[i].shadowMapYRes));

				for (int j = 2; j <= shadowBlurs; j++) {
					uFilterFBOToFBO(lightList[i].blurBuffers[0], sdShadowHBlur, lightList[i].shadowMapXRes, lightList[i].shadowMapYRes, 1, lightList[i].buffers[1]);
					uFilterFBOToFBO(lightList[i].buffers[0], sdShadowVBlur, lightList[i].shadowMapXRes, lightList[i].shadowMapYRes, 1, lightList[i].blurBuffers[1]);
				}
				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);
			}
		}
	}
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glViewport(0, 0, resSX, resSY);

	viewMatrix = eyeCam.getViewMatrix();
	projectionMatrix = eyeCam.getProjMatrix();
	uSetUniform2f(sdDeferred2, "RES", resSX, resSY);
	uSetUniform2f(sdBlueChroma, "RES", resSX, resSY);
	uSetUniform2f(sdDeferred2S, "RES", resSX, resSY);
	uSetUniform2f(sdDeferred1, "RES", resSX, resSY);
	uSetUniform2f(sdDeferred2, "range", eyeCam.zNear, eyeCam.zFar);
	uSetUniformMatrix4x4(sdDeferred2, "viewMatrix", viewMatrix);
	uSetUniformMatrix4x4(sdDeferred2, "invProjMatrix", glm::inverse(projectionMatrix));
	uSetUniform3f(sdDeferred2, "camPos", eyeCam.getPos());
	uSetUniform2f(sdDeferred2S, "range", eyeCam.zNear, eyeCam.zFar);
	uSetUniformMatrix4x4(sdDeferred2S, "viewMatrix", viewMatrix);
	uSetUniformMatrix4x4(sdDeferred2S, "viewMatrixInverse", glm::inverse(viewMatrix));
	uSetUniformMatrix4x4(sdDeferred2S, "invProjMatrix", glm::inverse(projectionMatrix));
	uSetUniform3f(sdDeferred2S, "camPos", eyeCam.getPos());
	uSetUniform2f(sdSSR, "range", eyeCam.zNear, eyeCam.zFar);
	uSetUniform2f(sdSSR, "RES", resSX, resSY);
	uSetUniformMatrix4x4(sdSSR, "invProjMatrix", glm::inverse(projectionMatrix));
	uSetUniformMatrix4x4(sdSSR, "projMatrix", projectionMatrix);

	renderStep();

	uRenderToFBO(gBuffer[0]);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer[1]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer[2]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gBuffer[5]);
	glActiveTexture(GL_TEXTURE0);
	uSetUniform1f(sdDeferred2S, "shadowBlurs", shadowBlurs);
	for (int i = 0; i < lightList.size(); i++) {
		glm::mat4 lightMatrix;
		lightMatrix = glm::translate(lightMatrix, lightList[i].position);
		setSphere(i, lightMatrix);
	}
	glDisable(GL_CULL_FACE);
	GLenum DrawBuffers2[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT4};
	glDrawBuffers(3, DrawBuffers2);
	
	if (glfwGetKey(window, GLFW_KEY_L) != GLFW_RELEASE)
		uFilterFBOToFBO(0, sdSSR, resSX, resSY, 5, gBuffer[3], gBuffer[2], gBuffer[4], gBuffer[5], blueNoiseTex);
	else
		uFilterFBOToFBO(0, sdGamma, resSX, resSY, 1, gBuffer[4]);
	//--------------------------------------------------------------------//
	glfwSwapBuffers(window);
	glfwPollEvents();
	deepTick++;
}

//Mode:
//0: G-Buffer writing
//1: Shadow maps
void renderGL(int mode) {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	uSetUniformMatrix4x4NoSwitch("modelViewProjMatrix", projectionMatrix * viewMatrix * modelMatrix);
	uSetUniformMatrix3x3NoSwitch("normalMatrix", glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix))));
	//uSetUniformMatrix4x4NoSwitch("modelViewMatrix", viewMatrix * modelMatrix);
	uSetUniformMatrix3x3NoSwitch("modelViewMatrix", glm::mat3(viewMatrix * modelMatrix));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex4);
	glActiveTexture(GL_TEXTURE0);
	//uSetUniform1fNoSwitch("diffuse_intensity", 0.5f);
	testModel.render();
	testModel2.render();
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	testMesh.display();
	for (int k = 0; k < 2; k++)
		for (int i = 0; i < 2; i++) {
			modelMatrix = glm::mat4();
			modelMatrix = glm::translate(modelMatrix, glm::vec3(i * 32, 0.5f, k * 32));
			modelMatrix = glm::rotate(modelMatrix, FrameSpeedCounter.framesPassed * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
			uSetUniformMatrix4x4NoSwitch("modelViewProjMatrix", projectionMatrix * viewMatrix * modelMatrix);
			uSetUniformMatrix3x3NoSwitch("normalMatrix", glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix))));
			uSetUniformMatrix4x4NoSwitch("modelViewMatrix", viewMatrix * modelMatrix);
			lattice.display();
		}

	glDisable(GL_CULL_FACE);
	modelMatrix = glm::mat4();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void setUpGL() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE0);
	glEnable(GL_TEXTURE1);
	glEnable(GL_TEXTURE2);
	glEnable(GL_TEXTURE3);
	glEnable(GL_TEXTURE4);
	glEnable(GL_TEXTURE5);
	glEnable(GL_TEXTURE6);
	glEnable(GL_TEXTURE7);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glActiveTexture(GL_TEXTURE0);
}

bool shouldClose() {
	return(glfwWindowShouldClose(window)) != 0;
}

void renderStep() {
	glDepthMask(GL_TRUE);
	uRenderToFBO(gBuffer[0]);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5};
	glDrawBuffers(4, DrawBuffers);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(sdWriteGBuffer);
	renderGL(0);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
}

void killWindow() {
	deleteFBOS();
	glfwDestroyWindow(window);
	glfwTerminate();
}