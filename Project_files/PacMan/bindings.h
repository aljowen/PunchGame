#pragma once

#include "SDL.h"

class Bindings {
public:
	int left;
	int right;
	int fire;
	int cameraPos;
	int perspective;

	bool leftPressed;
	bool rightPressed;
	bool firePressed;

	bool escapePressed;

	bool fullScreen = false;

	void loadDefaultBindings() {
		left = SDLK_a;
		right = SDLK_d;
		fire = SDLK_SPACE;
	}
};