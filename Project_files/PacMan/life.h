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

class Life : public GameObject {
public:

	int Update(float dt);
	void place(int num);

	Life::Life() :GameObject()
	{
		vertices.assign({
			-0.06f, -0.06f, 0.0f,	0.0f, 0.0f,		0.f, 0.f, 0.f,
			0.06f, -0.06f, 0.0f,	1.0f, 0.0f,		0.f, 0.f, 0.f,
			-0.06f, 0.06f, 0.0f,	0.0f, 1.0f,		0.f, 0.f, 0.f,
			0.06f, 0.06f, 0.0f,		1.0f, 1.0f,		0.f, 0.f, 0.f });
		indices.assign({
			0,1,2,
			1,3,2
		});

		SetupImg("assets/life.png");
		Setup();
	}
};