#pragma once

#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "2d.h"
#include "gameObject.h"
#include "bindings.h"

class Missile : public GameObject {
public:

	float moveSpeed = 2.0f;

	int Update(float dt);

};