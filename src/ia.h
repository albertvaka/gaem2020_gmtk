#pragma once

#include "input.h"
#include "player.h"
#include "selfregister.h"

struct Ia : Player, SelfRegister<Ia>
{
	Ia(int id);
	void Update(float dt);
	bool isGameKeyPressed(GameKeys);
	bool input_map[magic_enum::enum_count<GameKeys>()];

	float currentTime;
};
