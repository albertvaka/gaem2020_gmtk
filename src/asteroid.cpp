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

Asteroid::Asteroid(float size, vec initial_pos, vec initial_vel)
  : size(size), mass(2000*size), velocity(initial_vel),
    acceleration(), CircleEntity(initial_pos, 20*sqrt(size)),
    anim(AnimLib::ASTERVOID)
{}

void Asteroid::Update(float dt) {
  auto asteroids = Asteroid::GetAll();

  acceleration = vec();

  for (auto sol : Sol::GetAll()) {
      float dist = pos.Distance(sol->pos);
      float acceleration_scalar = sol->mass / (dist);
      vec acc_add = (sol->pos - pos).Normalized() * acceleration_scalar;
      acceleration += acc_add;
  }

  for (auto planet : Planet::GetAll()) {
      float dist = pos.Distance(planet->pos);
      float acceleration_scalar = planet->mass / (dist);
      vec acc_add = (planet->pos - pos).Normalized() * acceleration_scalar;
      acceleration += acc_add;
  }

  for (auto other : asteroids) {
    if (other != this) {
      float dist = pos.Distance(other->pos);
      float dist_bounds = bounds().Distance(other->bounds());

      float acceleration_scalar = other->mass / (dist);
      acceleration += (other->pos - pos).Normalized() * acceleration_scalar;

      if (dist_bounds <= 0) {
        vec own_vel = velocity
          - (2 * other->mass)/(other->mass + mass)
          * ((velocity - other->velocity).Dot(pos - other->pos) / (pos - other->pos).Length())
          * (pos - other->pos) * 0.2;
        vec their_vel = other->velocity
          - (2 * mass)/(other->mass + mass)
          * ((other->velocity - velocity).Dot(other->pos - pos) / (other->pos - pos).Length())
          * (other->pos - pos) * 0.2;

        velocity = own_vel;
        other->velocity = their_vel;
      }
    }
  }

  velocity += acceleration * dt;
  vec vel_dir = velocity.Normalized();
  float vel_sca = std::clamp(velocity.Length(), 0.0f, 160.0f);
  velocity = vel_dir * vel_sca;

  pos += velocity * dt;
  anim.Update(dt);
 
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
  const GPU_Rect& animRect = anim.GetCurrentRect();
  Window::Draw(Assets::asterVoidTexture, pos)
    .withOrigin(vec(animRect.w, animRect.h) / 2)
    .withRect(animRect)
    .withScale((20*sqrt(size))/(animRect.w/4));

  Window::DrawPrimitive::Line(pos, pos + acceleration, 3, 255, 0, 0);
  Window::DrawPrimitive::Line(pos, pos + velocity, 3, 0, 255, 0);
}
