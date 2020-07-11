#include "asteroid.h"

#ifdef _IMGUI
#include "imgui.h"
#endif

#include "anim_lib.h"
#include "assets.h"
#include "debug.h"
#include "planet.h"
#include "window.h"
#include "mates.h"

#include <float.h>

Asteroid::Asteroid(float mass, vec initial_pos, vec initial_vel)
  : mass(mass), velocity(initial_vel),
    acceleration(), CircleEntity(initial_pos, 10)
{}

void Asteroid::Update(float dt) {
  auto asteroids = Asteroid::GetAll();

  acceleration = vec();

  for (auto planet : Planet::GetAll()) {
      float dist = pos.Distance(planet->pos);
      float acceleration_scalar = planet->mass / ((dist * dist));
      vec acc_add = (planet->pos - pos).Normalized() * acceleration_scalar;
      acceleration += acc_add;
  }

  for (auto other : asteroids) {
    if (other != this) {
      float dist = pos.Distance(other->pos);
      float acceleration_scalar =  other->mass / (dist * dist);
      acceleration += (other->pos - pos).Normalized() * acceleration_scalar;
    }
  }

  velocity += 0.5 * acceleration * dt;
  vec vel_dir = velocity.Normalized();
  float vel_sca = std::clamp(velocity.Length(), 0.0f, 100.0f);
  velocity = vel_dir * vel_sca;

  pos += velocity * dt;

#ifdef _IMGUI
  if (ImGui::CollapsingHeader("Asteroid", true)) {
    float posv[2] = {pos.x, pos.y};
    ImGui::InputFloat2("Pos", posv);
    float velv[2] = {velocity.x, velocity.y};
    ImGui::InputFloat2("Vel", velv);
    float accv[2] = {acceleration.x, acceleration.y};
    ImGui::InputFloat2("Acc", accv);
  }
#endif

}

void Asteroid::Draw() const
{
  const GPU_Rect& rect = AnimLib::ASTEROID;
  Window::Draw(Assets::invadersTexture, pos)
    .withRect(rect)
    .withOrigin(vec(rect.w, rect.h)/2);

  Window::DrawPrimitive::Line(pos, pos + acceleration, 3, 255, 0, 0);
  Window::DrawPrimitive::Line(pos, pos + velocity, 3, 0, 255, 0);
}
