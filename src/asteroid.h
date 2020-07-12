#ifndef __ASTEROID_H_
#define __ASTEROID_H_

#include "animation.h"
#include "vec.h"
#include "entity.h"
#include "selfregister.h"

struct Asteroid : CircleEntity, SelfRegister<Asteroid>
{
  int player_id;
  vec position;
  vec velocity;
  vec acceleration;
  float size;
  float mass;
  Animation anim;

  static inline const float SizeToScaleMultiplier = 0.5f;

  float max_speed_mult = 1.0f;
  float time_in_sol = 0;

  Asteroid(int player_id, float size, vec initial_pos, vec initial_vel);

  void Update(float dt);
  void Draw() const;
};

#endif // __ASTEROID_H_
