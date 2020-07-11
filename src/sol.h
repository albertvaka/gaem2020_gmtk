#ifndef __SOL_H_
#define __SOL_H_

#include "animation.h"
#include "entity.h"
#include "selfregister.h"
#include "vec.h"

struct Sol : CircleEntity, SelfRegister<Sol>
{
  float mass = 12000;
  Animation solAnim;
  Sol(vec pos);

  void Update(float dt);
  void Draw() const;
};

#endif // __SOL_H_
