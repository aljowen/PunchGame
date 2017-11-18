#pragma once

#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <vector>

#include "2d.h"
#include "gameObject.h"
#include "bindings.h"
#include "missile.h"

class WorldBoundary : public GameObject {
public:

	int Update(float dt, Bindings keyBinds);
	void CreateGeometrySide();
	void CreateGeometryTop();

	WorldBoundary::WorldBoundary() :GameObject()
	{
		//Player::playerList.emplace_back(this);
	}
};