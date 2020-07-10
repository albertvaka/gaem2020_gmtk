#include "asteroid.h"

#ifdef _IMGUI
#include "imgui.h"
#endif

#include "anim_lib.h"
#include "assets.h"
#include "debug.h"
#include "planet.h"
#include "window.h"

Asteroid::Asteroid(float mass, vec initial_pos, vec initial_vel)
  : mass(mass), velocity(initial_vel),
    acceleration(50.0, 0), CircleEntity(initial_pos, 10)
{}

void Asteroid::Update(float dt) {
  auto asteroids = Asteroid::GetAll();

  for (auto planet : Planet::GetAll()) {
      float acceleration_scalar = planet->MASS / (pos.Distance(planet->pos));
      acceleration += (planet->pos - pos).Normalized() * acceleration_scalar;
  }

  for (auto other : asteroids) {
    if (other != this) {
      float acceleration_scalar = other->mass / (pos.Distance(other->pos));
      acceleration += (other->pos - pos).Normalized() * acceleration_scalar;
    }
  }

  velocity += 0.5 * acceleration * dt;
  pos += velocity* dt;

#ifdef _IMGUI
#endif
}

void Asteroid::Draw() const
{
  const GPU_Rect& rect = AnimLib::ASTEROID;
  Window::Draw(Assets::invadersTexture, pos)
    .withRect(rect)
    .withOrigin(vec(rect.w, rect.h)/2);
}
