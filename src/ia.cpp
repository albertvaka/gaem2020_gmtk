
#include "player.h"
#include "ia.h"

const float changeTime = 3.f;

Ia::Ia(int id) : Player(id), input_map()
{
	input_map[GameKeys::LEFT] = false;
	input_map[GameKeys::RIGHT] = false;
	input_map[GameKeys::SHOOT] = false;
	input_map[GameKeys::SHIELD] = false;
}

void Ia::Update(float dt)
{
	
	if (state == IAState::IDLE) {
		if (currentEnergy > 1.5) {
			state = IAState::ATTACK;
		}
	}
	else if (state == IAState::ATTACK) {
		
	}
	Player::Update(dt);
}

bool Ia::isGameKeyPressed(GameKeys gk) {
	return input_map[gk];
}