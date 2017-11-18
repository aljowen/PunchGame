#include "score.h"

void Score::updateTexture(std::string text) {

	SDL_Surface* scoreSurf;

	
	scoreSurf = TTF_RenderUTF8_Blended(font, text.c_str(), { 106,92,82 });
	//SDL_SetSurfaceAlphaMod(scoreSurf, 0.5);

	if (scoreSurf == NULL) {
		Logger::Error("Cannot create text surface '" + std::string(SDL_GetError()) + "'");
	}

	//Turns the surface into an image

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scoreSurf->w, scoreSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, scoreSurf->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(scoreSurf);

	return;
}