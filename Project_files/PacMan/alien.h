#pragma once

#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <chrono>

#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "2d.h"
#include "gameObject.h"
#include "missile.h"

class Alien : public GameObject {
public:
	//Alien defenitions
	float size = 0.1f;

	bool dead = false;
	float deadTime = 0.5;

	float animateNextFrame;

	float leftMostLoc;
	float rightMostLoc;
	float speed;
	float acceleration;

	int moveDownCounter = 5;

	int Update(float dt, std::vector<Missile> &missileList, std::vector<Alien> &alienList, int &shootCount, std::vector<Missile> &alienMissileList);

	Alien::Alien() :GameObject()
	{
		/*alienList.emplace_back(this);*/
	}
	void Place(int x, int y);
};