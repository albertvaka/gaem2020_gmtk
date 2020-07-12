
#include "player.h"
#include "planet.h"
#include "ia.h"
#include "text.h"
#include "assets.h"
#include "asteroid.h"
#include "collide.h"
#include "rand.h"

const float changeTime = 3.f;
const float attackChangeTime = 2.f;


Ia::Ia(int id, Player* enemy) : Player(id), enemy(enemy), state(IAState::IDLE), input_map()
{
	input_map[GameKeys::LEFT] = false;
	input_map[GameKeys::RIGHT] = false;
	input_map[GameKeys::SHOOT] = false;
	input_map[GameKeys::SHIELD] = false;
}

vec Ia::generateNewAttackVector() {
	vec enemyDirection = enemy->planet->pos - planet->pos;
	float direction = Random::roll(2) == 1 ? 1 : -1;
	float var = Random::rollf(Mates::DegsToRads(1), Mates::DegsToRads(15));
	float rotation = (Mates::DegsToRads(35.f) + var) * direction;
	return enemyDirection.RotatedAroundOrigin(rotation).Normalized();
}

void Ia::goToTarget(vec v) {
	vec rel_pos = (pos - planet->pos).RotatedAroundOrigin(M_PI).Normalized();
	if (rel_pos.Dot(v) > -0.9) {
		float sign = rel_pos.Cross(v);
		if (sign > 0.f) {
			input_map[GameKeys::RIGHT] = false;
			input_map[GameKeys::LEFT] = true;
		}
		else if (sign < 0.f) {
			input_map[GameKeys::RIGHT] = true;
			input_map[GameKeys::LEFT] = false;
		}
	}
	else {
		input_map[GameKeys::RIGHT] = false;
		input_map[GameKeys::LEFT] = false;
	}
}

void Ia::Update(float dt)
{
	//Movement
	if (currentAttackChangeTime <= 0.f) {
		attackVector = generateNewAttackVector();
		currentAttackChangeTime = changeTime + Random::rollf(-1.9f, 1.5f);
	}
	else {
		currentAttackChangeTime -= dt;
	}
	goToTarget(attackVector);

	//shooting
	if (waitingChargeTime > 0.f) { 
		waitingChargeTime -= dt;
	}
	else {
		input_map[GameKeys::SHOOT] = true;
		if (nextCharge - shotCharge < 0.1f) {
			input_map[GameKeys::SHOOT] = false;
			nextCharge = Random::rollf(0.1f, currentEnergy);
			waitingChargeTime = Random::rollf(3.f);
		}
	}

	//shield
	float points = 0.f;
	CircleBounds shieldArea(pos, shieldInfluence);
	for (Asteroid* asteroid : Asteroid::GetAll()) {
		vec relPos = (asteroid->pos - planet->pos).Normalized();
		vec dir = asteroid->acceleration.Normalized();
		//Debug::out << relPos.Dot(dir);
		if (relPos.Dot(dir) > 0.7f) {
			if (Collide(shieldArea, asteroid->bounds())) {
				points += 10 * asteroid->size;
			}
		}
	}
	if (points >= 8.f) {
		input_map[GameKeys::SHIELD] = true;
	}
	else {
		input_map[GameKeys::SHIELD] = false;
	}
	
	Player::Update(dt);
}

void Ia::Draw() const
{
	Player::Draw();
	/*
	if (Debug::Draw) {
		Window::DrawPrimitive::Line(planet->pos, planet->pos + attackVector, 2, 255, 0, 0);
	}
	*/
}

bool Ia::isGameKeyPressed(GameKeys gk) {
	return input_map[gk];
}
