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
  float mass;

  Asteroid(float mass, vec initial_pos, vec initial_vel);

  void Update(float dt);
  void Draw() const;
};

#endif // __ASTEROID_H_
