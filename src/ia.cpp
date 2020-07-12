
#include "player.h"
#include "ia.h"

const float changeTime = 3.f;

Ia::Ia(int id) : Player(id), currentTime(changeTime), input_map()
{
	input_map[GameKeys::UP] = false;
	input_map[GameKeys::DOWN] = false;
	input_map[GameKeys::LEFT] = false;
	input_map[GameKeys::RIGHT] = false;
}

void Ia::Update(float dt)
{
	currentTime -= dt;
	if (currentTime < 0.f) {
		input_map[GameKeys::LEFT] = !input_map[GameKeys::LEFT];
		input_map[GameKeys::RIGHT] = input_map[GameKeys::LEFT];
		currentTime = changeTime;
	}
	Player::Update(dt);
}

bool Ia::isGameKeyPressed(GameKeys gk) {
	return input_map[gk];
}