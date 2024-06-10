
#include "FPSCounter.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>

void FPSCounter::tick() {
	double timeGet = glfwGetTime();
	deltaTime = timeGet - currentTime;
	currentTime = timeGet;
	nbFrames++;
	framesPassed++;
	if (currentTime - lastTime >= 1.0){
		//std::cout << 1000.0 / double(nbFrames) << " ms/Frame " << double(nbFrames) << " FPS" << std::endl;
		FPS = double(nbFrames);
		FPS = deltaTime*1000.0;
		nbFrames = 0;
		lastTime += 1.0;
	}
}

FPSCounter::FPSCounter() {
	fprintf(stdout, "FPSCounterMKI Initalized!\n\n");
}