#include "asteroid.h"

#ifdef _IMGUI
#include "imgui.h"
#endif

#include "anim_lib.h"
#include "assets.h"
#include "debug.h"
#include "window.h"

Asteroid::Asteroid(float mass, vec initial_pos, vec initial_vel)
  : mass(mass), velocity(initial_vel),
    acceleration(50.0, 0), CircleEntity(initial_pos, 10)
{}

void Asteroid::Update(float dt) {
  auto asteroids = Asteroid::GetAll();

  for (auto other : asteroids) {
    if (other != this) {
      float acceleration_scalar = other->mass / (pos.Distance(other->pos));
      vec dir = (pos - other->pos).Normalized();
      acceleration = dir * acceleration_scalar;
    }
  }

  velocity += 0.5 * acceleration * dt;
  pos += velocity* dt;

#ifdef _IMGUI
#endif
}

void Asteroid::Draw() const
{
  const GPU_Rect& animRect = AnimLib::BULLET;
  Window::Draw(Assets::invadersTexture, pos)
    .withRect(animRect)
    .withOrigin(vec(animRect.w, 0)/2);
}
