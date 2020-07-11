#pragma once

#include "entity.h"
#include "animation.h"

struct Planet;

struct Player : Entity
{
	int id;

	float angle;
	float cannonAngle;
	float angularVel = 0.f;
	float shotCharge = -1.f;
	vec shotPos;
	bool invertControlsX = false;
	bool invertControlsY = false;
	Planet* planet;
	Animation asteroidAnim;
	int owner_planet;

	Player(int id, int owner_planet);

	void Update(float dt);
	void Draw() const;
};
