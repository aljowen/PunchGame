#pragma once

#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm.hpp"

#include <vector>
#include <memory>

#include "2d.h"



//All in game objects are gameObjects, however game object should never be used, only its children
class GameObject {
public:
	//Use this to access list of all game objects
	//static std::vector<std::shared_ptr<GameObject>> objList;
	//static std::vector<GameObject*> objList;
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;;

	glm::mat4 modelMatrix;
	
	//Contains all of the vertices of this particular game object
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	GLuint tex;

	virtual int Update() { return 0; };
	void Render(GLuint* shaderProgram);


	void Setup();
	void SetupImg(std::string imageLoc);

	void loadModel(std::string filePath);
private:

};


