#pragma once

#include "entity.h"
#include "selfregister.h"


struct Planet : CircleEntity, SelfRegister<Planet>
{
	const static inline float INITIAL_HEALTH = 100.f;

	float mass;
	float orbit_radius;
	float health;
	float rps;
	float curr_angle;
	int planetTexture = 1;

	Planet(float orbit_radius, float orbit_offset, float mass, float rps);

	void Update(float dt);
	void Draw() const;
};
