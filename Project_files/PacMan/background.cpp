#include "background.h"

int Background::Update(float playerX) {

	modelMatrix = glm::mat4();
	modelMatrix[3].x = - ( playerX / 2.0f );
	return 0;
}