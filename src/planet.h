#pragma once

#include "entity.h"
#include "selfregister.h"


struct Planet : CircleEntity, SelfRegister<Planet>
{
	const static inline float SCALE = 0.7f;

	const static inline float INITIAL_HEALTH = 100.f;

	float mass;
	float orbit_radius;
	float health;
	float rps;
	float curr_angle;

	Planet(float orbit_radius, float orbit_offset, float mass, float rps);

	void Update(float dt);
	void Draw() const;
};
