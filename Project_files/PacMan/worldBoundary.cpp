
#include "worldBoundary.h"

int WorldBoundary::Update(float dt, Bindings keyBinds) {

	

	return 0;
}

void WorldBoundary::CreateGeometrySide() {
	/*vertices.assign({
		0.02f , 0.98f , 0.0f,		1.0f, 1.0f,		0.f, 0.f, 0.f,
		0.02f , -0.98f , 0.0f,		1.0f, 0.0f,		0.f, 0.f, 0.f,
		-0.02f , -0.98f , 0.0f,		0.0f, 0.0f,		0.f, 0.f, 0.f,
		-0.02f , 0.98f , 0.0f,		0.0f, 1.0f, 	0.f, 0.f, 0.f });
	indices.assign({
		0,1,2,
		0,2,3 });*/
	loadModel("Models\Side.dae");

	return;
}

void WorldBoundary::CreateGeometryTop() {
	vertices.assign({
		1.0f , 0.02f , 0.0f,		1.0f, 1.0f,		0.f, 0.f, 0.f,
		1.0f , -0.02f , 0.0f,		1.0f, 0.0f,		0.f, 0.f, 0.f,
		-1.0f , -0.02f , 0.0f,		0.0f, 0.0f,		0.f, 0.f, 0.f,
		-1.0f , 0.02f , 0.0f,		0.0f, 1.0f,		0.f, 0.f, 0.f });
	indices.assign({
		0,1,2,
		0,2,3 });

	return;
}

