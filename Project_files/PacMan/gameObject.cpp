#include <glew.h>
#include <SDL.h>
#include "SDL_opengl.h"
#include <gl\GLU.h>

#include "SDL_image.h"

#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); //WARN: using hard-coded locatoin
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		
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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void GameObject::loadModel(std::string filePath) {
	//Loads the 3D model
	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(filePath, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		Logger::Error(import.GetErrorString());
		return;
	}

	//Gets the Vertices and UV's and pushes them to the vertices list
	for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {
		glm::vec3 tempVec;
		tempVec.x = scene->mMeshes[0]->mVertices[i].x;
		tempVec.y = scene->mMeshes[0]->mVertices[i].y;
		tempVec.z = scene->mMeshes[0]->mVertices[i].z;

		glm::vec2 tempTexCo;
		if (scene->mMeshes[0]->mTextureCoords[0]) {
			tempTexCo.x = scene->mMeshes[0]->mTextureCoords[0][i].x;
			tempTexCo.y = scene->mMeshes[0]->mTextureCoords[0][i].y;
		}

		glm::vec3 tempVecNorm;
		tempVecNorm.x = scene->mMeshes[0]->mNormals[i].x;
		tempVecNorm.y = scene->mMeshes[0]->mNormals[i].y;
		tempVecNorm.z = scene->mMeshes[0]->mNormals[i].z;

		vertices.insert(vertices.end(),{ tempVec.x,tempVec.y ,tempVec.z ,tempTexCo.x ,tempTexCo.y, tempVecNorm.x,tempVecNorm.y ,tempVecNorm.z });
	}

	//Gets indices from the mesh and adds them to the indices vector
	for (int i = 0; i < scene->mMeshes[0]->mNumFaces; i++) {
		aiFace face = scene->mMeshes[0]->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
}