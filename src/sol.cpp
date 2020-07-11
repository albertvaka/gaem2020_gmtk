#include "sol.h"

#include "assets.h"
#include "asteroid.h"
#include "window.h"

Sol::Sol(vec pos, float radius)
  : CircleEntity(pos, radius), solAnim(AnimLib::BLACKHOLE)
{
}

void Sol::Update(float dt)
{
  solAnim.Update(dt);
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
	float scale = 200 / 300.0f;
	const GPU_Rect& animRect = solAnim.GetCurrentRect();
	Window::Draw(Assets::solTexture, pos)
		.withRect(animRect)
		.withScale(scale)
		.withOrigin(vec(animRect.w, animRect.h) / 2);
}
