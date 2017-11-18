#include "life.h"

int Life::Update(float dt) {

	return 0;
}

void Life::place(int num) {

	//Use the offsetX and Y to place the aliens using the model matrix
	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, glm::vec3( ( 1.4 * ( float(num) / 10.0f) + 0.659f ), -1.09f, 0.0f));

	return;
}