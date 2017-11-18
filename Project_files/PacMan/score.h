#pragma once

#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "SDL_ttf.h"

#include "2d.h"
#include "gameObject.h"
#include "console.h";

class Score : public GameObject {
public:

	TTF_Font *font = nullptr;

	void updateTexture(std::string text);

};