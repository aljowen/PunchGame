
#include "player.h"

int Player::Update(float dt, Bindings keyBinds) {

	//Disables the player if they have no health left
	//if (playerHealth.size() > 0) {
		

		//Player movement
		if (keyBinds.leftPressed) {
			if (modelMatrix[3].x > -0.8f) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(-moveSpeed * dt, 0.0f, 0.0f));
			}
		}
		if (keyBinds.rightPressed) {
			if (modelMatrix[3].x < 0.8f) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(moveSpeed * dt, 0.0f, 0.0f));
			}
		}

		if (keyBinds.firePressed) {
			fireWeapon();
		}
	//}
	

	return -1;
}

void Player::fireWeapon() {


	return;
}

//std::vector<std::shared_ptr<Player>> Player::playerList;