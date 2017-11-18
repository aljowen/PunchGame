#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#define GLM_FORCE_RADIANS
#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "2d.h"
#include <list>
#include <vector>


#include "console.h"
#include "game.h"



//Entry point of game code
int main(int argc, char *args[])
{
	Logger::Info("Game Starting");
	
	//Game startup
	bool play = true;
	bool exit = false;

	Game g;
	g.Init();
	g.Load();

	//Game runtime
	
	while (exit == false) {
		g.GameLoop(play);
		//Currently no menu system so sets exit to to true when play is set to false
		if (play == false) {
			exit = true;
		}
	}

	
	//Writes the log to file, may take some time depending on log file size
	Logger::Out();
	return 0;
}




