#include "planet.h"

#include "assets.h"
#include "window.h"
#include "asteroid.h"
#include "collide.h"

const float size = 50.0;

Planet::Planet(vec pos, float health): 
	health(health), CircleEntity(pos, size)
{
}

void Planet::Update(float dt) 
{
	auto asteroids = Asteroid::GetAll();

	for (auto asteroid : asteroids) {
		if (Collide(this, asteroid)) {
			health -= asteroid->mass;
			asteroid->alive = false;
		}
	}
}

void Planet::Draw() const
{
	float scale = size / 160;
	Window::Draw(Assets::backgroundTexture, pos)
		.withOrigin(Assets::backgroundTexture->w / 2, Assets::backgroundTexture->h / 2)
		.withScale(scale);

	if (Debug::Draw) {
		DrawBounds(100, 100, 255);
	}
}
