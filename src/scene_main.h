#pragma once

#include "mates.h"
#include "tilemap.h"
#include "scene.h"
#include "partsys.h"
#include "input.h"
#include "player.h"

struct SceneMain : Scene {

	Player player;
	PartSys alienPartSys;
	int currentLevel = 1;

	SceneMain();

	void EnterScene() override;
	void ExitScene() override;
	void Update(float dt) override;
	void Draw() override;

	void SpawnAliens();


};
