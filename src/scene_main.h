#pragma once

#include "mates.h"
#include "tilemap.h"
#include "scene.h"
#include "partsys.h"
#include "input.h"
#include "player.h"
#include "planet.h"

struct SceneMain : Scene {

	Planet planet_one;
	Planet planet_two;

	SceneMain();

	void EnterScene() override;
	void ExitScene() override;
	void Update(float dt) override;
	void Draw() override;


};
