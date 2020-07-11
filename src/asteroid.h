#ifndef __ASTEROID_H_
#define __ASTEROID_H_

#include "vec.h"
#include "entity.h"
#include "selfregister.h"

struct Asteroid : CircleEntity, SelfRegister<Asteroid>
{
  vec position;
  vec velocity;
  vec acceleration;
  float size;
  float mass;

  Asteroid(float mass, vec initial_pos, vec initial_vel);

  static float GetSpriteScale(float size) {
	  return 0.2f + size / 2.f;
  }

  void Update(float dt);
  void Draw() const;
};

#endif // __ASTEROID_H_
