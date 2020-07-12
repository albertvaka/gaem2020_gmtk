#pragma once

#include "input.h"
#include "player.h"
#include "selfregister.h"

enum IAState {
	IDLE,
	ATTACK
};

struct Ia : Player, SelfRegister<Ia>
{
	Ia(int id, Player* enemy);
	void Update(float dt);
	void Draw() const;
	bool isGameKeyPressed(GameKeys);
	bool input_map[magic_enum::enum_count<GameKeys>()];
	Player* enemy;

	IAState state;

	void goToTarget(vec v);

	// --Attacking--
	float currentAttackChangeTime = 0.f;
	vec attackVector = vec(1, 0);
	vec generateNewAttackVector();
	float waitingChargeTime = 1.f;
	float nextCharge = 1.f;

};
