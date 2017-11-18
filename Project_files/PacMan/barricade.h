#pragma once

#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm.hpp"

#include "2d.h"
#include "gameObject.h"
#include "missile.h"

class Barricade : public GameObject {
public:
	//Use this to access list of all players
	//static std::vector<std::shared_ptr<Player>> playerList;

	int Update(float dt, std::vector<Missile> &missileList, std::vector<Missile> & alienMissileList);

	Barricade::Barricade() :GameObject()
	{
		//Player::playerList.emplace_back(this);
	}

	void Place(int x, int sx, int sy);
};