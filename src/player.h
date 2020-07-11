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
	int currentLoad;
	float currentLoadTime;
	float currentShieldTime;
	vec shotPos;
	Planet* planet;
	Animation asteroidAnim;

	Player(int id);

	void Update(float dt);
	void Draw() const;
};
