
#include "barricade.h"

int Barricade::Update(float dt, std::vector<Missile> &missileList, std::vector<Missile> & alienMissileList) {


	//Detects if it has been hit by a player missile
	for (int i = 0; i < missileList.size(); i++) {
		if ((fabs(missileList[i].modelMatrix[3].x - modelMatrix[3].x) < 0.05) && (fabs(missileList[i].modelMatrix[3].y - modelMatrix[3].y) < 0.05)) {
			missileList.erase(missileList.begin() + i);
			return i;
		}
	}

	//Detects if it has been hit by an alien missile
	for (int i = 0; i < alienMissileList.size(); i++) {
		if ((fabs(alienMissileList[i].modelMatrix[3].x - modelMatrix[3].x) < 0.05) && (fabs(alienMissileList[i].modelMatrix[3].y - modelMatrix[3].y) < 0.05)) {
			alienMissileList.erase(alienMissileList.begin()+i);
			return i;
		}
	}


	return -1;
}

void Barricade::Place(int x, int sx, int sy) {

	float offsetX = (x * 0.40) - 0.44;

	float offsetSX = 0.08f * float(sx);

	float offsetSY = 0.04f * float(sy);

	loadModel("Models/barricade.dae");

	/*vertices.assign({
		-0.04f, -0.02f, 0.0f,	float(sx) / 2.f,		float(sy) / 4.f,
		0.04f, -0.02f, 0.0f,	float(sx) / 2.f + 0.5f,	float(sy) / 4.f,
		-0.04f, 0.02f, 0.0f,	float(sx) / 2.f,		float(sy) / 4.f + 0.25f,
		0.04f, 0.02f, 0.0f,		float(sx) / 2.f + 0.5f, float(sy) / 4.f + 0.25f });
	indices.assign({
		0,1,2,
		1,2,3});*/

	//Use the offsetX and Y to place the aliens using the model matrix
	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, glm::vec3(offsetX + offsetSX, -0.7 + offsetSY, 0.0f));

}