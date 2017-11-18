#include <glew.h>
#include <SDL.h>
#include "SDL_opengl.h"
#include <gl\GLU.h>

#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <chrono>
#include <fstream>

#include "game.h"
#include "console.h"
#include "player.h"
#include "background.h"

#include "bindings.h"

//"{ gl_Position =  projectionMat * viewMat * modelMat *  vec4(position.x, position.y, position.z, 1.0);\n"

const GLchar* vertexShaderSource =
"#version 440 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texCoord;\n"
"layout (location = 2) in vec3 normal;\n"
"out vec2 TexCoord;\n"
"uniform mat4 modelMat = mat4(1.0);\n"
"uniform mat4 viewMat = mat4(1.0);\n"
"uniform mat4 projectionMat = mat4(1.0);\n"

"out vec3 Normal;\n"
"out vec3 FragPos;\n"
"out vec3 viewPos;\n"

"void main()\n"
"{ gl_Position = projectionMat * viewMat * modelMat *  vec4(position, 1.0);\n"
"TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);\n"
"FragPos = vec3(modelMat * vec4(position, 1.0f));\n"
"Normal = mat3(transpose(inverse(modelMat))) * normal;\n"
"viewPos = vec3(viewMat[3].x, viewMat[3].y, viewMat[3].z);}";


const GLchar* fragmentShaderSource =
"#version 440 core\n"

"out vec4 color;\n"

"in vec2 TexCoord;\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"

"uniform vec3 lightPos;\n"
"in vec3 viewPos;\n"
"uniform vec4 lightColor = vec4(1.0f,1.0f,1.0f,0);\n"
"uniform sampler2D ourTexture;\n"

"void main()\n"
"{\n"
"float ambientStrength = 0.6f;\n"
"float ambient = ambientStrength;\n"

"float diffuseStrength = 1.0f;\n"
"vec3 norm = normalize(Normal);\n"
"vec3 lightDir = normalize(lightPos - FragPos);\n"
"float diff = diffuseStrength * max(dot(norm, lightDir),0.0);\n"
//"vec4 diffuse = diff * lightColor;\n"


"float specularStrength = 0.5f;\n"
"vec3 viewDir = normalize(viewPos - FragPos);\n"
"vec3 reflectDir = reflect(-lightDir, norm);\n"
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"float specular = specularStrength * spec;\n"

"color = texture(ourTexture, TexCoord) * (vec4(ambient,ambient,ambient,1) + vec4(diff,diff,diff,1) + vec4(specular,specular,specular,1));}"
//"color = texture(ourTexture, TexCoord) * (ambient + diff + specular);}"
;
	

//Inititalises all of the things required to make the game run
int Game::Init() {

	// SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Error("Cannot initialise. Error is '" + std::string(SDL_GetError()) + "'");
		return 1;
	}
	Logger::Info("SDL initialised OK!");


	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	width = dm.w/2;
	height = dm.h/2;

	// Window Creation
	win = SDL_CreateWindow("Alwyn Owen; CGP2012M-1617; 14519159", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w / 2, dm.h / 2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (win == nullptr) {
		Logger::Error("Cannot create Window. Error is '" + std::string(SDL_GetError()) + "'");
		return 1;
	}


	// Renderer Creation
	SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (SDL_GL_CreateContext(win) == NULL) {
		Logger::Error("Cannot create Window Context. Error is '" + std::string(SDL_GetError()) + "'");
	}
	Logger::Info("OpenGL version: " + std::string(""));
	Logger::Info("OpenGL initialised OK!");

	//Setting up glew
	glewExperimental = GL_TRUE;
	GLenum temp = glewInit();
	if (GLEW_OK != temp) {
		SDL_Quit();
		exit(1);
	}
	Logger::Info("glew initialised OK!");


	// SDL_Image initialise
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		Logger::Error("Cannot initialise images. Error is '" + std::string(SDL_GetError()) + "'");
		return 1;
	}
	Logger::Info("SDL_image initialised OK!");

	// SDL_ttf initialise
	if (TTF_Init() == -1) {
		Logger::Error("Cannot initialise ttf. Error is '" + std::string(SDL_GetError()) + "'");
		return 1;
	}
	Logger::Info("SDL_ttf initialised OK!");

	// SDL_mixer initialise
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		Logger::Error("Cannot initialise audio. Error is '" + std::string(Mix_GetError()) + "' [Check audio hardware is connected...]");
		return 1;
	}
	Logger::Info("SDL_mixer initialised OK!");


	//Setting OpenGL texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);





	//Creates vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Gets the source file and assigns it to the vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compiles the shader
	glCompileShader(vertexShader);

	//Finds out if the compilation was succesful, if not gets error and logs it
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[500];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Logger::Error("Vertex shader compilation failed" + std::string(infoLog));
	}

	//Creates fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Gets the source file and assigns it to the fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//Compiles the shader
	glCompileShader(fragmentShader);

	//Finds out if the compilation was succesful, if not gets error and logs it
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[500];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		Logger::Error("Fragment shader compilation failed" + std::string(infoLog));
	}

	//Link shaders
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[500];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		Logger::Error("Shader linking failed" + std::string(infoLog));
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//TODO lookup locations

	return 0;
}


//Loads and creates any assets required for the game
int Game::Load() { 

	
	alienShootCounter = 3000;

	//Setting up bindings
	keyBinds.loadDefaultBindings();

	//Loading game settings
	LoadOptions();

	//Creating world boundaries
		//Left
	/*	WorldBoundary left;
		left.CreateGeometrySide();
		left.SetupImg("assets/border.png");
		left.Setup();
		left.modelMatrix = glm::mat4();
		left.modelMatrix = glm::translate(left.modelMatrix, glm::vec3(-0.98f, 0.0f, 0.0f));
		objList.push_back(left);*/

		////Right
		//WorldBoundary right;
		//right.CreateGeometrySide();
		//right.SetupImg("assets/border.png");
		//right.Setup();
		//right.modelMatrix = glm::mat4();
		//right.modelMatrix = glm::translate(right.modelMatrix, glm::vec3(0.98f, 0.0f, 0.0f));
		//objList.push_back(right);

		////Top
		//WorldBoundary top;
		//top.CreateGeometryTop();
		//top.SetupImg("assets/border.png");
		//top.Setup();
		//top.modelMatrix = glm::mat4();
		//top.modelMatrix = glm::translate(top.modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
		//objList.push_back(top);

		////Bottom
		//WorldBoundary bottom;
		//bottom.CreateGeometryTop();
		//bottom.SetupImg("assets/border.png");
		//bottom.Setup();
		//bottom.modelMatrix = glm::mat4();
		//bottom.modelMatrix = glm::translate(bottom.modelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
		//objList.push_back(bottom);

		

	//Setting up background
		back1.vertices.assign({
			//main section
			-30.f, -30.f, -20.f,	0.0f, 0.0f,		0.f, 0.f, 0.f, //0 bottom left
			30.f, -30.f, -20.f,		1.0f, 0.0f,		0.f, 0.f, 0.f, //1 bottom right
			-30.f, 30.f, -20.f,		0.0f, 1.0f,		0.f, 0.f, 0.f, //2 top left
			30.f, 30.f, -20.f,		1.0f, 1.0f,		0.f, 0.f, 0.f  //3 top right
			
		});
		back1.indices.assign({
			//main
			0,1,2,
			1,3,2
		});
		back1.SetupImg("assets/background.png");
		back1.Setup();

	//Setting up foreground flash
		flash.vertices.assign({
			-3.f, -3.f, 0.0f,	0.0f, 0.0f,		0.f, 0.f, 0.f,
			3.f, -3.f, 0.0f,	1.0f, 0.0f,		0.f, 0.f, 0.f,
			-3.f, 3.f, 0.0f,	0.0f, 1.0f,		0.f, 0.f, 0.f,
			3.f, 3.f, 0.0f,		1.0f, 1.0f,		0.f, 0.f, 0.f });
		flash.indices.assign({
			0,1,2,
			1,3,2
		});
		flash.SetupImg("assets/border.png");
		flash.Setup();


	//Creating player
	Player player1;

	player1.loadModel("Models/spaceShip.dae");

	player1.SetupImg("Models/spaceShipUV.png");
	player1.Setup();
	player1.modelMatrix = glm::mat4();
	player1.modelMatrix = glm::translate(player1.modelMatrix, glm::vec3(0.0f, -0.9f, 0.0f));
	playerList.push_back(player1);


	//Setting up screen perspective
	projectionMatrix = glm::perspective(glm::radians(fov), 16.0f / 9.0f, 0.01f, 50.0f);
	glUseProgram(shaderProgram);

	//projectionMatrix = glm::mat4(1.0);
	GLint projectionLocation = glGetUniformLocation(shaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glUseProgram(shaderProgram);
	//Setting up the camera position
	viewMatrix = glm::mat4();

	//set up camera variables
	//Position = 3 units along the positive z axis
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	//Look at from that position = (0,0,-1)
	glm::vec3 camLook = glm::vec3(0.0f, 0.0f, -1.0f);
	//set the Up axis for the camera
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//Sets the game to use the default camera
	keyBinds.cameraPos = 1;
	//Sets the default projection to use
	keyBinds.perspective = 1;

	viewMatrix = glm::lookAt(camPosition,camPosition+camLook, camUp);

	GLint viewLocation = glGetUniformLocation(shaderProgram, "viewMat");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	return 0;
}


void Game::LoadOptions() {
	std::vector<std::string> file;

	//Opens file
	std::ifstream optFile;
	optFile.open("./options.ini");

	//error check, if file can't be opened assume 60fps
	if (optFile.is_open() == false) {
		Logger::Error("Cannot open options file. Using default values");
		vsync = 1;
		frameLimiter = 1;
		frameTarget = 60;
		fov = 60;
		aspectLock = 1;
		scale = 0;
		return;
	}

	//Copys all of the files contents to file vector
	while (!optFile.eof()) {
		std::string line;
		optFile >> line;
		file.push_back(line);
	}

	//closes options file
	optFile.close();

	//Uses data from file to set values
	vsync = ( stoi(file[0].substr(6)) != 0 );
	frameLimiter = ( stoi(file[1].substr(13)) != 0 );
	frameTarget = stoi(file[2].substr(11));
	fov = stoi(file[3].substr(4));
	aspectLock = stoi(file[4].substr(11));
	scale = stoi(file[5].substr(14));
	return;
}


//The main gameplay loop
int Game::GameLoop(bool &play) {
	Logger::Info("Game Loop Entered");

	float dt = 1.0f/frameTarget;

	while (play) {

		std::chrono::high_resolution_clock::time_point timeStart = std::chrono::high_resolution_clock::now();

		//If the game does not update for over 0.3 seconds plus the normal frame time it will pause the physics
		if (dt > 1.0f / frameTarget + 0.3f) {
			dt = 1.0f / frameTarget + 0.3f;
		}

		//INPUT UPDATE RENDER
		Input(play);
		Update(dt , play);
		Render();

		//Time keeping for dt
		float time = (std::chrono::high_resolution_clock::now() - timeStart).count() / 1000000000.0f;

		if (frameLimiter == true){
			//If time is negative, the time probably overflew, ignore it and continue
			if (time < 0) continue;

			//Calculates whether the game hit its target, if it did it delays to keep to frame limit, if not it carrys on
			float sleepTime = (1.0 / (float)frameTarget) - (time);
			if (sleepTime > 0)
			{
				SDL_Delay(sleepTime);
				Logger::Perf("Slept for: " + std::to_string(sleepTime));
			}
			else {
				Logger::Perf("FROZE FOR: " + std::to_string(-sleepTime) + " !!!");
			}
		}
		

		dt = (std::chrono::high_resolution_clock::now() - timeStart).count() / 1000000000.0f;

	}
	Logger::Info("Game Loop Exited");
	return 0;
}


//Input stage of gameplay loop
int Game::Input(bool &play) {


	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//WINDOW EVENTS
		if (event.type == SDL_QUIT) {
			play = false;
			break;
		}
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
			SDL_DisplayMode dm;
			SDL_GetCurrentDisplayMode(0, &dm);

			//Drawing to screen stuff
			glUseProgram(shaderProgram);

			int intWidth, intHeight;

			SDL_GetWindowSize(win, &intWidth, &intHeight);

			width = intWidth;
			height = intHeight;
		}

		//When key lifted
		if (event.type == SDL_KEYUP) {
			//KEY INPUTS

			if (event.key.keysym.sym == SDLK_f) {
				//Allows user to fullscreen
				if (keyBinds.fullScreen) {
					SDL_SetWindowFullscreen(win, 0);
					keyBinds.fullScreen = false;
				}
				else {
					SDL_DisplayMode dm;
					SDL_GetCurrentDisplayMode(0, &dm);
					SDL_SetWindowSize(win, dm.w, dm.h);
					SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
					
					//Setting up screen perspective
					width = dm.w;
					height = dm.h;

					SDL_SetWindowDisplayMode(win, &dm);
					keyBinds.fullScreen = true;
				}
			}
			if (event.key.keysym.sym == SDLK_1) {
				keyBinds.cameraPos = 1;
			}
			if (event.key.keysym.sym == SDLK_2) {
				keyBinds.cameraPos = 2;
			}
			if (event.key.keysym.sym == SDLK_3) {
				keyBinds.cameraPos = 3;
			}
			if (event.key.keysym.sym == SDLK_o) {
				if (keyBinds.perspective == 1) {
					keyBinds.perspective = 2;
				}
				else {
					keyBinds.perspective = 1;
				}
			}
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				keyBinds.escapePressed = true;
				break;
			}
			else
			{
				keyBinds.escapePressed = false;
			}

			//GAME INPUT
			if (event.key.keysym.sym == keyBinds.left) {
				if (keyBinds.leftPressed == true) {
					keyBinds.leftPressed = false;
				}
			}
			if (event.key.keysym.sym == keyBinds.right) {
				if (keyBinds.rightPressed == true) {
					keyBinds.rightPressed = false;
				}
			}
			if (event.key.keysym.sym == keyBinds.fire) {
				if (keyBinds.firePressed == true) {
					keyBinds.firePressed = false;
				}
			}
		}
		//When key pressed
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == keyBinds.left) {
				keyBinds.leftPressed = true;
			}
			if (event.key.keysym.sym == keyBinds.right) {
				keyBinds.rightPressed = true;
			}
			if (event.key.keysym.sym == keyBinds.fire) {
				keyBinds.firePressed = true;
			}
		}
	}

	Logger::Perf("Input Complete");
	return 0;
}



//Process stage of gameplay loop
int Game::Update(float dt, bool &play) {

	//Allows for player to close game
	if (keyBinds.escapePressed) {
		play = false;
	}


	//loop through all game objects that need to update

	//Players
	for (unsigned int i = 0; i < Game::playerList.size(); i++) {
		Game::playerList[i].Update(dt,keyBinds);
	}


	//background
	back1.Update(playerList[0].modelMatrix[3].x);

	Logger::Perf("Update Complete");
	return 0;
}



//Render stage of gameplay loop
int Game::Render() {

	//glClearColor(0.3,0.3,0.3,1.0);

	//glClear(GL_COLOR_BUFFER_BIT);


	/*GLint viewLocation = glGetUniformLocation(shaderProgram, "viewMat");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));*/

	//Drawing to screen stuff
	glUseProgram(shaderProgram);


	//scale multiplyer
	float multX, multY;

	if (aspectLock) {
		if (width / height == (16.f / 9.f)) {
			multX = width;
			multY = height;
		}
		if (width / height > (16.f / 9.f)) {
			multX = height * 1.7777777777778f;
			multY = height;
		}
		if (width / height < (16.f / 9.f)) {
			multX = width;
			multY = width / 1.7777777777778f;
		}
	}
	else {
		multX = width;
		multY = height;
	}
	


	//Setting up screen perspective
	switch (keyBinds.perspective)
	{
	case 1:
		if (scale) {
			projectionMatrix = glm::perspective(glm::radians(fov), width / height, 0.01f, 50.0f);
		}
		else {
			projectionMatrix = glm::perspective(glm::radians(fov), 16.0f / 9.0f, 0.01f, 50.0f);
		}
		break;
	case 2:
		projectionMatrix = glm::ortho(0.0f, 4.0f, 0.0f, 2.25f, 0.01f, 100.0f);
		break;
	}
	glViewport(0,0,multX,multY);

	glUseProgram(shaderProgram);

	//projectionMatrix = glm::perspective(glm::radians(45.0f), GLfloat(800 /600), 0.1f, 100.0f);
	//projectionMatrix = glm::mat4(1.0);
	GLint projectionLocation = glGetUniformLocation(shaderProgram, "projectionMat");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//Setting up the camera position
	viewMatrix = glm::mat4();

	//Case statement for moving camera on y axis when player is hit
	float cameraYMod = 0;
	switch (hit)
	{
	case 0:
		cameraYMod = 0.0;
		break;
	case 5:
		cameraYMod = 0.01;
		hit--;
		break;
	case 4:
		cameraYMod = 0.04;
		hit--;
		break;
	case 3:
		cameraYMod = 0.03;
		hit--;
		break;
	case 2:
		cameraYMod = 0.02;
		hit--;
		break;
	case 1:
		cameraYMod = 0.01;
		hit--;
		break;
	}

	//Variables used for setting the camera's position
	glm::vec3 camPosition;
	glm::vec3 camLook;
	glm::vec3 camUp;
	//Selecting the right camera position to use
	switch (keyBinds.cameraPos)
	{
	case 1:
		//set up camera variables
		camPosition = glm::vec3((playerList[0].modelMatrix[3].x / 4.0f), (playerList[0].modelMatrix[3].y / 4.0f) + 0.2f + cameraYMod, 2.0f);
		//Look at from that position = (0,0,-1)
		camLook = glm::vec3(0.0f, 0.0f, -1.0f);
		//set the Up axis for the camera
		camUp = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 2:
		//set up camera variables
		camPosition = glm::vec3((playerList[0].modelMatrix[3].x / 4.0f), (playerList[0].modelMatrix[3].y / 4.0f) - 1.0f + cameraYMod, 0.7f);
		//Look at from that position = (0,0,-1)
		camLook = glm::vec3(0.0f, 1.0f, -1.0f);
		//set the Up axis for the camera
		camUp = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 3:
		//set up camera variables
		camPosition = glm::vec3((playerList[0].modelMatrix[3].x), (playerList[0].modelMatrix[3].y) + 0.05f + cameraYMod, 0.01f);
		//Look at from that position = (0,0,-1)
		camLook = glm::vec3(0.0f, 3.142f, -1.0f);
		//set the Up axis for the camera
		camUp = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	}

	//Puts the UI at the camera

	//roundText.modelMatrix[3].x = camPosition.x;
	//roundText.modelMatrix[3].y = camPosition.y;
	//roundText.modelMatrix[3].z = camPosition.z;

	//roundText.modelMatrix = glm::rotate(roundText.modelMatrix, camLook.y - cameraPrevRotY, glm::vec3(1.0f, 0.0f, 0.0f));

	cameraPrevRotY = camLook.y;

	//roundText.modelMatrix = glm::lookAt(camPosition, camPosition + camLook, camUp);
	

	
	//Corrects the camera position for Ortho view
	if(keyBinds.perspective == 2){
		camPosition.x -= 2.0f;
		camPosition.y -= 1.125f;
	}

	viewMatrix = glm::lookAt(camPosition, camPosition + camLook, camUp);

	GLint viewLocation = glGetUniformLocation(shaderProgram, "viewMat");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::vec3 lightPos(0.0f, 0.0f, 2.0f);

	GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, playerList[0].modelMatrix[3].x, playerList[0].modelMatrix[3].y + 0.0f, playerList[0].modelMatrix[3].z + 0.0f);

	//#########################################
	//Rendering stuff
	//#########################################

	back1.Render(&shaderProgram);
	//scoreText.Render(&shaderProgram);
	//roundText.Render(&shaderProgram);

	//random game objects
	for (unsigned int i = 0; i < Game::objList.size(); i++) {
		Game::objList[i].Render(&shaderProgram);
	}

	//Players
	if (playerList.size() != 0) {
		for (unsigned int i = 0; i < Game::playerList.size(); i++) {
			Game::playerList[i].Render(&shaderProgram);
		}
	}

	

	//screen flash
	if (hit ==2 || hit==3 || hit == 4 || hit == 5) {
		flash.Render(&shaderProgram);
	}

	glUseProgram(0);
	SDL_GL_SwapWindow(win);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Logger::Perf("Render Complete");
	return 0;
}