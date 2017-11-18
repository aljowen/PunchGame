#pragma once

#include <vector>

#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm.hpp"

#include "gameObject.h"
#include "player.h"
#include "background.h"


#include "bindings.h"

class Game {
public:

	SDL_Window *win = nullptr;
	GLuint shaderProgram;

	int frameTarget;
	bool frameLimiter;
	bool vsync;
	float fov;
	bool aspectLock;
	bool scale;

	float cameraPrevRotY = 0.0f;

	int alienShootCounter;

	int score = 0;
	int round = 1;

	int hit = 0;

	float width = 1280;
	float height = 720;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	std::vector<GameObject> objList;
	std::vector<Player> playerList;

	Background back1;
	Background flash;

	Bindings keyBinds;

	int Init();
	int Load();
	void LoadOptions();

	int GameLoop(bool &play);

	int Input(bool &play);
	int Update(float dt, bool &play);
	int Render();
};