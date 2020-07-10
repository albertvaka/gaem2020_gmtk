#pragma once
#include "entity.h"
struct Planet : CircleEntity
{
	const float SCALE = 0.7;

	vec pos;
	float health;

	Planet(vec pos, float health);

	void Update(float dt);
	void Draw() const;
};

