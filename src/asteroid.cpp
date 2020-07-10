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
    acceleration(), CircleEntity(initial_pos, 10)
{}

void Asteroid::Update(float dt) {
  auto asteroids = Asteroid::GetAll();

  for (auto planet : Planet::GetAll()) {
      float dist = pos.Distance(planet->pos);
      float acceleration_scalar = 0.1 * planet->MASS / (dist * dist);
      acceleration += (planet->pos - pos).Normalized() * acceleration_scalar;
  }

  for (auto other : asteroids) {
    if (other != this) {
      float dist = pos.Distance(other->pos);
      float acceleration_scalar =  0.1 * other->mass / (dist * dist);
      acceleration += (other->pos - pos).Normalized() * acceleration_scalar;
    }
  }

  // Accelerate towards the center of the map so asteroids don't leave mamma
  vec screen_half = vec(Window::GAME_WIDTH/2, Window::GAME_HEIGHT/2);

  velocity += 0.5 * acceleration * dt * dt;
  pos += velocity* dt;

#ifdef _IMGUI
  if (ImGui::CollapsingHeader("Asteroid", true)) {
    float posv[2] = {pos.x, pos.y};
    ImGui::InputFloat2("Pos", posv);
    float velv[2] = {velocity.x, velocity.y};
    ImGui::InputFloat2("Vel", velv);
    float accv[2] = {acceleration.x, acceleration.y};
    ImGui::InputFloat2("Vel", accv);
  }
#endif
}

void Asteroid::Draw() const
{
  const GPU_Rect& animRect = AnimLib::BULLET;
  Window::Draw(Assets::invadersTexture, pos)
    .withRect(animRect)
    .withOrigin(vec(animRect.w, 0)/2);

  Window::DrawPrimitive::Line(pos, pos + acceleration, 3, 255, 0, 0);
  Window::DrawPrimitive::Line(pos, pos + velocity, 3, 0, 255, 0);
}
