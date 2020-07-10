#include "Planet.h"

#include "input.h"
#include "assets.h"
#include "animation.h"
#include "bullet.h"
#include "window.h"
Planet::Planet(vec pos, float health): 
	pos(pos), health(health)
{
}

void Planet::Update(float dt) 
{
}

void Planet::Draw() const
{
	Window::Draw(Assets::backgroundTexture, pos)
		.withOrigin(Assets::backgroundTexture->w / 2, Assets::backgroundTexture->h / 2)
		.withScale(SCALE);
		
}
