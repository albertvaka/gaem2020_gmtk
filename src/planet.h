#pragma once

#include "entity.h"
#include "selfregister.h"


struct Planet : CircleEntity, SelfRegister<Planet>
{
	const float SCALE = 0.7;
	const float MASS = 1000000;

	float health;

	Planet(vec pos, float health);

	void Update(float dt);
	void Draw() const;
};
