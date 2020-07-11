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
      asteroid->velocity = ast_dir * 120.0f;
    }
  }
}

void Sol::Draw() const
{
	float scale = 80 / 160.0f;
	Window::Draw(Assets::backgroundTexture, pos)
		.withOrigin(Assets::backgroundTexture->w / 2, Assets::backgroundTexture->h / 2)
		.withScale(scale);
}
