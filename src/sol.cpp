#include "sol.h"

#include "assets.h"
#include "asteroid.h"
#include "window.h"

Sol::Sol(vec pos, float radius)
  : CircleEntity(pos, radius)
{
}

void Sol::Update(float dt)
{
  for (auto asteroid : Asteroid::GetAll()) {
    if (asteroid->bounds().Distance(bounds()) < 0) {
      vec ast_dir = asteroid->velocity.Normalized();
      asteroid->velocity = ast_dir * 2000;
      asteroid->max_speed_mult = 1.5f;
    }
  }
}

void Sol::Draw() const
{
	float scale = 80 / 160.0f;
	Window::Draw(Assets::oldPlanetTexture, pos)
		.withOrigin(Assets::oldPlanetTexture->w / 2, Assets::oldPlanetTexture->h / 2)
		.withScale(scale);
}
