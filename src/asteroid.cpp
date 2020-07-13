#include "asteroid.h"

#ifdef _IMGUI
#include "imgui.h"
#endif

#include "anim_lib.h"
#include "assets.h"
#include "debug.h"
#include "planet.h"
#include "sol.h"
#include "window.h"
#include "mates.h"
#include "rand.h"

#include <float.h>

Asteroid::Asteroid(int player_id, float size, vec initial_pos, vec initial_vel)
  : player_id(player_id), size(size), mass(sqrt(size)*7000), velocity(initial_vel),
    acceleration(), CircleEntity(initial_pos, 20*sqrt(size)),
    anim(player_id == 0?AnimLib::ASTERVOID1 : AnimLib::ASTERVOID2)
{}

void Asteroid::Update(float dt) {
  auto asteroids = Asteroid::GetAll();

  acceleration = vec();

  for (auto sol : Sol::GetAll()) {
      float dist = pos.Distance(sol->pos);
      float acceleration_scalar = sol->mass / (dist);
      vec acc_add = (sol->pos - pos).Normalized() * acceleration_scalar;
      acceleration += acc_add;

      if (bounds().Distance(sol->bounds()) <= 0) {
        time_in_sol += dt;
      }
      else {
        time_in_sol = 0.0f;
      }
  }

  for (auto planet : Planet::GetAll()) {
      float dist = pos.Distance(planet->pos);
      float acceleration_scalar = planet->mass / (dist);
      vec acc_add = (planet->pos - pos).Normalized() * acceleration_scalar;
      acceleration += 1.4f * acc_add;
  }

  for (auto other : asteroids) {
    if (other != this) {
      float dist = pos.Distance(other->pos);
      float acceleration_scalar = other->mass / (dist);
      acceleration += 1.15f * (other->pos - pos).Normalized() * acceleration_scalar;
    }
  }

  velocity += acceleration * dt;
  vec vel_dir = velocity.Normalized();
  float vel_sca = std::clamp(velocity.Length(), 0.0f, 340.0f/(std::fmax(size, 1.f)) * max_speed_mult);
  velocity = vel_dir * vel_sca;

  if (max_speed_mult > 1.0f) {
    max_speed_mult *= 0.999f;
  }
  if (max_speed_mult < 1.0f) {
    max_speed_mult = 1.0f;
  }

  if (time_in_sol > 1.5f) {
    alive = false;
  }

  pos += velocity * dt;
  anim.Update(dt);
 
#ifdef _IMGUI
  //if (ImGui::CollapsingHeader("Asteroid", true)) {
  //  float posv[2] = {pos.x, pos.y};
  //  ImGui::InputFloat2("Pos", posv);
  //  float velv[2] = {velocity.x, velocity.y};
  //  ImGui::InputFloat2("Vel", velv);
  //  float accv[2] = {acceleration.x, acceleration.y};
  //  ImGui::InputFloat2("Acc", accv);
  //}
#endif

}

void Asteroid::Draw() const
{
  const GPU_Rect& animRect = anim.GetCurrentRect();
  Window::Draw(Assets::asterVoidTexture, pos)
    .withOrigin(vec(animRect.w, animRect.h) / 2)
    .withRect(animRect)
    .withScale(1.5f*sqrt(size));

  if (Debug::Draw) {
      Window::DrawPrimitive::Line(pos, pos + acceleration, 3, 255, 0, 0);
      Window::DrawPrimitive::Line(pos, pos + velocity, 3, 0, 255, 0);
  }
}
