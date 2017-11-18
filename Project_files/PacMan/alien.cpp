
#include "alien.h"

int Alien::Update(float dt, std::vector<Missile> &missileList, std::vector<Alien> &alienList, int &shootCount, std::vector<Missile> &alienMissileList) {

	if (dead == false) {
		//If the alien reaches its outer movemetn barrier move in opposite direction
		if (modelMatrix[3].x > rightMostLoc) {
			//Negate the speed
			speed = -speed;
			//Reset the position to be on the border
			modelMatrix[3].x = rightMostLoc;
			//Decrement the y count down counter
			moveDownCounter--;
		}
		if (modelMatrix[3].x < leftMostLoc) {
			//Negate the speed
			speed = -speed;
			//Reset the position to be on the border
			modelMatrix[3].x = leftMostLoc;
			//Decrement the y count down counter
			moveDownCounter--;
		}

		//Checks if the alien needs to move down
		if (moveDownCounter <= 0) {
			moveDownCounter = 5;
			modelMatrix[3].y = modelMatrix[3].y - 0.015;
		}

		//checks if an alien has moved down far enough to end the game
		if (modelMatrix[3].y <= -0.5) {
			return -2;
		}

		//Keep speed increasing through game
		if (speed > 0) {
			speed += acceleration;
		}
		else {
			speed -= acceleration;
		}

		//Move the alien to its new position
		modelMatrix = glm::translate(modelMatrix, glm::vec3(speed*dt, 0.0f, 0.0f));


		//Detects if it has been hit by a player missile
		for (int i = 0; i < missileList.size(); i++) {
			if ((fabs(missileList[i].modelMatrix[3].x - modelMatrix[3].x) < 0.05) && (fabs(missileList[i].modelMatrix[3].y - modelMatrix[3].y) < 0.05)) {
				dead = true;
				return i;
			}
		}


		bool shoot = true;

		//Figures out if it needs to shoot
		//Checks for any aliens below
		for (int i = 0; i < alienList.size(); i++) {
			if (alienList[i].modelMatrix[3].y - modelMatrix[3].y < -0.05) {
				if (fabs(alienList[i].modelMatrix[3].x - modelMatrix[3].x) < 0.06) {
					//Checks if it needs to shoot
					shoot = false;
				}
			}
		}

		if (shoot) {
			shootCount--;
			if (shootCount <= 0) {
				//Creates a missile
				Missile newMis;

				newMis.loadModel("Models/Bullet.dae");

				/*newMis.vertices.assign({
					0.02f , 0.02f , 0.0f,		1.0f, 1.0f,
					0.02f , -0.02f , 0.0f,		1.0f, 0.0f,
					-0.02f , -0.02f , 0.0f,		0.0f, 0.0f,
					-0.02f , 0.02f , 0.0f,		0.0f, 1.0f });
				newMis.indices.assign({
					0,1,2,
					0,2,3 });*/

				newMis.modelMatrix = modelMatrix;
				newMis.moveSpeed = -1.0f;

				newMis.SetupImg("Models/EnemyMissile.png");
				newMis.Setup();
				alienMissileList.push_back(newMis);

				shootCount = 999;
			}
		}
	}
	else {
		if (deadTime >= 0) {
			modelMatrix = glm::rotate(modelMatrix, 4.2f * dt, glm::vec3(0.0f,1.0f,1.0f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -speed*dt, 0.0f));
			deadTime = deadTime - dt;
		}
		else {
			return -3;
		}
		
	}
	

	//Does sprite animation by adjusting the texture mapping
	/*float time = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	if ( time >= animateNextFrame) {
		animateNextFrame = time + 10000000;
		if (vertices[3] >= 1.0f) {
			vertices[3] = 0.0f;
			vertices[8] = 0.0f;
			vertices[13] = 0.0f;
			vertices[18] = 0.0f;
		}
		else {
			vertices[3] += (1 / 8);
			vertices[8] += (1 / 8);
			vertices[13] += (1 / 8);
			vertices[18] += (1 / 8);
		}
	}*/
	

	return -1;
}

void Alien::Place(int x, int y) {
	
	//Calculate the correct placement for alien
	float offsetX = (x * 0.12) - 0.66;
	float offsetY = (y * 0.12);

	//Calculate the limits for the aliens movement
	leftMostLoc = offsetX - 0.15f;
	rightMostLoc = offsetX + 0.15f;

	//Set the speed and acceleration
	speed = 0.06f;
	acceleration = 0.0002;

	//Assign the vertices and indices for the alien
	loadModel("Models/alien.dae");

	/*vertices.assign({
		0.04f , 0.04f , 0.0f,		1.0f, 1.0f,
		0.04f , -0.04f , 0.0f,		1.0f, 0.0f,
		-0.04f , -0.04f , 0.0f,		0.0f, 0.0f,
		-0.04f , 0.04f , 0.0f,		0.0f, 1.0f });
	indices.assign({
		0,1,2,
		0,2,3});*/

	//Use the offsetX and Y to place the aliens using the model matrix
	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, glm::vec3(offsetX, offsetY, 0.0f));

}