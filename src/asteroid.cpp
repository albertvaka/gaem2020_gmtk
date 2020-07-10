#include "asteroid.h"

#include "anim_lib.h"
#include "assets.h"
#include "debug.h"
#include "window.h"

Asteroid::Asteroid(float mass, vec initial_pos, vec initial_vel)
  : mass(mass), position(initial_pos),
    velocity(initial_vel), acceleration(),
    CircleEntity(pos, 10)
{}

void Asteroid::Update(float dt) {
  auto asteroids = Asteroid::GetAll();

  Debug::out << "HOLA";
  for (auto asteroid : asteroids) {
    if (asteroid != this) {
      // TODO(Marce): Accumulate acceleration towards other bodies
      acceleration +=
    }
  }

  // TODO(Marce): Update speed

}

void Asteroid::Draw() const
{
  const GPU_Rect& animRect = AnimLib::BULLET;
  Window::Draw(Assets::invadersTexture, pos)
    .withRect(animRect)
    .withOrigin(vec(animRect.w, 0)/2);
}
