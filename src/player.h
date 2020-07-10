#pragma once

#include "entity.h"

struct Player : Entity
{
	int id;
	vec planet_center;
	float angle;
	float angularVel;
	bool invertControls;

	Player(int id, vec planet_center);

	void Update(float dt);
	void Draw() const;
};
