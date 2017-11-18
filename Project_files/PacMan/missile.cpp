#include "missile.h"

int Missile::Update(float dt) {

	//move the missile
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, moveSpeed * dt, 0.0f));

	//Returns 1 if it has hit a world border
	if (modelMatrix[3].y > 1.0f) {
		return 1;
	}
	if (modelMatrix[3].y < -1.0f) {
		return 1;
	}
	return 0;
}