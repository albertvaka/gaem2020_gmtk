#pragma once

#include "entity.h"

struct Player : Entity
{
	int id;
	vec planet_center;
	float angle;
	float cannonAngle;
	float angularVel = 0.f;
	bool invertControlsX = false;
	bool invertControlsY = false;


	Player(int id, vec planet_center);

	void Update(float dt);
	void Draw() const;
};
