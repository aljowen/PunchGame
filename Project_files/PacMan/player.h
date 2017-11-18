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

class Player : public GameObject {
public:

	float moveSpeed = 0.5f;

	int Update(float dt, Bindings keyBinds);

	Player::Player() :GameObject()
	{
		
	}
};