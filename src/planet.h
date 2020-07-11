#pragma once

#include "entity.h"
#include "selfregister.h"


struct Planet : CircleEntity, SelfRegister<Planet>
{
	const float SCALE = 0.7;

	float mass;
	float orbit_radius;
	float health;
	float rps;
	float curr_angle;

	Planet(float orbit_radius, float orbit_offset, float mass, float health, float rps);

	void Update(float dt);
	void Draw() const;
};
