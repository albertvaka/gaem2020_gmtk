#include "planet.h"

#include "rand.h"
#include "assets.h"
#include "window.h"
#include "asteroid.h"
#include "collide.h"

const float size = 50.0;

Planet::Planet(float orbit_radius, float orbit_offset, float mass, float rps)
	: orbit_radius(orbit_radius)
	, mass(mass)
	, health(INITIAL_HEALTH)
	, CircleEntity(vec(), size)
	, rps(rps)
	, curr_angle(orbit_offset)
	, txt_health(Assets::font_30)
{
	planetTexture = Random::roll(std::size(Assets::planetTextures));
	txt_health.setString(std::to_string(int(ceil(health))));
}

void Planet::Update(float dt) 
{
	this->curr_angle += rps * dt;
	pos = vec(Window::GAME_WIDTH/2, Window::GAME_HEIGHT/2)
		+ vec::FromAngle(Mates::DegsToRads(curr_angle), orbit_radius);

	auto asteroids = Asteroid::GetAll();

	for (auto asteroid : asteroids) {
		if (Collide(this, asteroid)) {
			health -= 10*asteroid->size;
			if (health < 0) health = 0;
			txt_health.setString(std::to_string(int(ceil(health))));
			asteroid->alive = false;
		}
	}
}

void Planet::Draw() const
{
	GPU_Image* texture = Assets::planetTextures[planetTexture];
	float scale = size /texture->base_h*2;
	
	Window::Draw(texture, pos)
		.withOrigin(texture->w / 2, texture->h / 2)
		.withScale(scale);

	if (health >= 0) {
		Window::Draw(txt_health, pos + vec(0.f, size * 0.5f + 10.0f))
			.withOrigin(txt_health.getSize().x, 0.f)
			.withScale(0.5f);
	}

	if (Debug::Draw) {
		DrawBounds(100, 100, 255);
	}
}
