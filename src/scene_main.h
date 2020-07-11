#pragma once

#include <vector>

#include "mates.h"
#include "tilemap.h"
#include "scene.h"
#include "partsys.h"
#include "input.h"
#include "player.h"
#include "planet.h"

struct SceneMain : Scene {

	std::vector<Planet> planets;
	// Planet planet_one;
	// Planet planet_two;
	Player player1;
	Player player2;

	PartSys alienPartSys;
	int currentLevel = 1;

	SceneMain();

	void EnterScene() override;
	void ExitScene() override;
	void Update(float dt) override;
	void Draw() override;
};
