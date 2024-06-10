
#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

class FPSCounter {
private:
	double currentTime = 0.0;
	double lastTime = 0.0;
	int nbFrames = 0;
public:
	float framesPassed = 0.f;
	double deltaTime = 0.0;
	double FPS = 0.0;
	FPSCounter();
	void tick();
};

#endif