#pragma once
#include "entity.h"
struct Planet : CircleEntity
{
	float health;

	Planet(vec pos, float health);

	void Update(float dt);
	void Draw() const;
};

