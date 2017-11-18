#include <glew.h>
#include <SDL.h>
#include "SDL_opengl.h"
#include <gl\GLU.h>

#include "SDL_image.h"

#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "gameObject.h"
#include "console.h"

//std::vector<std::shared_ptr<GameObject>> GameObject::objList;
//std::vector<GameObject*> GameObject::objList;



void GameObject::Setup() {
//once per load object
	//Linking vertex stuff
		
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		
		//Giving the vertices to the GPU buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		//CHECK if vertices empty?
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW); //TEST what happens if vertices is empty

		//Giving the connected elements to the GPU buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		//Sets that drawing will be done in tirangles, starting with element 0 as defined at end variable
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); //WARN: using hard-coded locatoin
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		//Puts object in a default position
		//modelMatrix = glm::mat4();
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
}



void GameObject::SetupImg(std::string imageLoc) {
	SDL_Surface* image = IMG_Load(imageLoc.c_str());
	if (image == NULL) {
		Logger::Error("Could not load surface");
	}
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image);
}

void GameObject::Render(GLuint* shaderProgram) {
	glBindTexture(GL_TEXTURE_2D, tex);
	glBindVertexArray(VAO);

	GLint modelLocation = glGetUniformLocation(*shaderProgram, "modelMat");
	glUniformMatrix4fv(modelLocation,1,GL_FALSE,glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}