
#include "player.h"

#include "planet.h"
#include "input.h"
#include "assets.h"
#include "animation.h"
#include "window.h"
#include "asteroid.h"

const float distanceFromPlanet = 100.f;
const float shotSpawnDistance = 30.f;

const float cannonMaxAngle = 75.f;

const float shotChargeSpeed = 0.5f;
const float shotMinCharge = 0.1f;
const float shotMaxCharge = 2.5f;

const float cannonVel = 200.f;
const float accel = 300.f;
const float maxVel = 500.f;
const float friction = 0.95f;

extern float mainClock;

Player::Player(int id)
	: id(id)
	, angle(90.f)
	, cannonAngle(0.f),
	asteroidAnim(AnimLib::ASTERVOID)
{
}

void Player::Update(float dt)
{
	// SHIP MOVEMENT

	vec l_stick = GamePad::AnalogStick::Left.get(id, 50.f);
	l_stick.Normalize();

	if (!l_stick.isZero()) {
		vec rel_pos = pos - planet->pos;
		rel_pos.Normalize();

		if (rel_pos.Dot(l_stick) < 0.99) {
			float sign = rel_pos.Cross(l_stick);
			if (sign > 0.f) {
				angularVel += accel * dt;
			}
			else if (sign < 0.f) {
				angularVel += -accel * dt;
			}
		}
	}

	Mates::Clamp(angularVel, -maxVel, maxVel);

	angle += angularVel * dt;

	if (angle > 360.f) angle -= 360.f;
	else if (angle < 0.f) angle += 360.f;

	angularVel *= friction;

	pos = planet->pos + vec::FromAngle(Mates::DegsToRads(angle)) * distanceFromPlanet;


	// CANON MOVEMENT

	vec r_stick = GamePad::AnalogStick::Right.get(id, 50.f);
	r_stick.Normalize();
	float stickAngle = r_stick.Angle() + 180;

	float target = (r_stick.isZero()) ? angle : stickAngle;

	float diff = fmod(target - cannonAngle, 360);

	if (id == 0) {
		Debug::out << diff;
	}

	if (diff > 10.f) {
		cannonAngle += cannonVel * dt;
	}
	else if (diff < -10.f) {
		cannonAngle -= cannonVel*dt;
	}
	else {
		cannonAngle = target;
	}

	if (cannonAngle > 360.f) cannonAngle -= 360.f;
	else if (cannonAngle < 0.f) cannonAngle += 360.f;

	//Mates::Clamp(cannonAngle, -cannonMaxAngle, cannonMaxAngle);
	pos.Debuggerino();

	// SHOOT

	if (shotCharge >= 0.f) {
		asteroidAnim.Update(dt);
		shotCharge += shotChargeSpeed * dt;
		if (shotCharge > shotMaxCharge) shotCharge = shotMaxCharge;
		
		
		shotPos = pos + vec::FromAngle(Mates::DegsToRads(cannonAngle)) * shotSpawnDistance;

		if ((Input::IsReleased(id, GameKeys::SHOOT) && shotCharge > shotMinCharge) || shotCharge >= shotMaxCharge) {
			new Asteroid(shotCharge, shotPos, vec::FromAngle(Mates::DegsToRads(cannonAngle)) * 300);
			shotCharge = -1.f;
		}

	}
	else if (Input::IsJustPressed(id, GameKeys::SHOOT)) {
		shotCharge = 0.f;
	}
}

void Player::Draw() const
{

	const GPU_Rect& cannonRect = AnimLib::CANNON;
	Window::Draw(Assets::invadersTexture, pos)
		.withRect(cannonRect)
		.withColor(id == 0 ? SDL_Color{ 0,255,255,255 } : SDL_Color{ 255, 255, 0, 255 })
		.withOrigin(vec(cannonRect.w, cannonRect.h) / 2)
		.withRotation(cannonAngle - 90);


	if (shotCharge > 0.f) {
		const GPU_Rect& animRect = asteroidAnim.GetCurrentRect();
		Window::Draw(Assets::asterVoidTexture, shotPos)
			.withRect(animRect)
			.withScale(20*sqrt(shotCharge) / (animRect.w / 4))
			.withOrigin(vec(animRect.w, animRect.h) / 2);
	}
}
