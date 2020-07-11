#pragma once

#include <vector>

#include "mates.h"
#include "tilemap.h"
#include "scene.h"
#include "partsys.h"
#include "input.h"
#include "player.h"
#include "planet.h"
#include "rototext.h"

struct SceneMain : Scene {

	Player player1;
	Player player2;

	RotoText rototext;

	PartSys alienPartSys;
	int currentLevel = 1;

	SceneMain();

	void EnterScene() override;
	void ExitScene() override;
	void Update(float dt) override;
	void Draw() override;
};
