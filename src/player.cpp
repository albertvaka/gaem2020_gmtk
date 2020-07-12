
#include "player.h"
#include "planet.h"
#include "input.h"
#include "assets.h"
#include "animation.h"
#include "window.h"
#include "asteroid.h"
#include "text.h"
#include "collide.h"

const float distanceFromPlanet = 100.f;
const float shotSpawnDistance = 30.f;

const float cannonMaxAngle = 75.f;

const float shotChargeSpeed = 0.7f;
const float shotMinCharge = 0.1f;
const float shotMaxCharge = 2.5f;

const float cannonVel = 300.f;
const float accel = 1000.f;
const float maxVel = 2000.f;
const float friction = 0.9f;

extern float mainClock;
const float maxEnergy = 1.5 * sqrt(shotMaxCharge);
const float loadTime = 3.f;
const float shieldTime = 5.f;
const float shieldInfluence = 200.f;
// Energy/second 
const float chargingRate = .25f;
Player::Player(int id)
	: id(id)
	, angle(90.f)
	, cannonAngle(0.f),
	currentEnergy(maxEnergy),
	asteroidAnim(AnimLib::ASTERVOID)
{
}

void Player::Update(float dt)
{
	// SHIP MOVEMENT

	vec l_stick = GamePad::AnalogStick::Left.get(id, 50.f);
	l_stick.Normalize();

	if (!l_stick.isZero()) {
		vec rel_pos = vec(0, 1);//pos - planet->pos;
		rel_pos.Normalize();

		//if (rel_pos.Dot(l_stick) < 0.99) {
			float sign = rel_pos.Cross(l_stick);
			if (sign < 0.f) {
				angularVel += accel * dt;
			}
			else if (sign > 0.f) {
				angularVel += -accel * dt;
			}
		//}
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

	if ((target > 180 && cannonAngle < 180 && diff > 0) || (cannonAngle > 180 && target < 180 && diff < 0)) {
		diff = -diff;
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
	if (currentEnergy < maxEnergy) {
		currentEnergy += dt * chargingRate;
	}
	if (shotCharge >= 0.f) {
		asteroidAnim.Update(dt);
		shotCharge += shotChargeSpeed * dt;
		if (shotCharge > shotMaxCharge) shotCharge = shotMaxCharge;
		
		shotPos = pos + vec::FromAngle(Mates::DegsToRads(cannonAngle)) * shotSpawnDistance;

		if ((Input::IsReleased(id, GameKeys::SHOOT) && shotCharge > shotMinCharge) || shotCharge >= shotMaxCharge || currentEnergy <= sqrt(shotCharge)) {
			new Asteroid(shotCharge, shotPos, vec::FromAngle(Mates::DegsToRads(cannonAngle)) * 30000);
			currentEnergy -= sqrt(shotCharge);
			shotCharge = -1.f;
		}

	}
	else if (Input::IsPressed(id, GameKeys::SHOOT)) {
		if (currentEnergy > sqrt(shotMinCharge)) {
			shotCharge = 0.f;
		}
	}
	if (currentShieldTime > 0) {
		currentShieldTime -= dt;
	}
	else if (Input::IsJustPressed(id, GameKeys::SHIELD)) {
		currentShieldTime = shieldTime;
		CircleBounds shieldArea(pos, shieldInfluence);
		for (Asteroid* asteroid : Asteroid::GetAll()) {
			if (Collide(shieldArea, asteroid->bounds())) {
				vec outwards_dir = (asteroid->pos - pos).Normalized();

				asteroid->velocity += 1000.0f * outwards_dir;
				asteroid->max_speed_mult = 2.5f;
			}
		}
	}
}

//https://gizma.com/easing
float easeOutQuad(float time, float start, float change, float duration) {
	time /= duration;
	return -change * time * (time - 2) + start;
}

void Player::Draw() const
{

	if (shotCharge > 0.f) {
		const GPU_Rect& animRect = asteroidAnim.GetCurrentRect();
		Window::Draw(Assets::asterVoidTexture, shotPos)
			.withRect(animRect)
			.withScale(20*sqrt(shotCharge) / (animRect.w / 4))
			.withOrigin(vec(animRect.w, animRect.h) / 2);
	}

	if (currentShieldTime <= 0) {
		Window::DrawPrimitive::Circle(pos, shieldInfluence, 1.5f, 50, 205, 50);
	}
	else {
		const float currentTime = shieldTime - currentShieldTime;
		const float radius = easeOutQuad(currentTime, 2.f, shieldInfluence, shieldTime);
		Window::DrawPrimitive::Circle(pos, radius, .5f, 90, 90, 90);
	}

	Window::Draw(id == 0? Assets::ship1Texture : Assets::ship2Texture, pos)
		.withColor(id == 0 ? SDL_Color{ 0,255,255,255 } : SDL_Color{ 255, 255, 0, 255 })
		.withRect({fabs(angularVel) > 50.f ? 64.f : 0.f, 0.f, 64.f, 64.f })
		.withScale(angularVel < 50.f ? 1.f : -1.f, 1.f)
		.withOrigin(vec(32.f, 32.f))
		.withRotation(cannonAngle + 90.f);

	// TODO: remove and use visual elements
	const float energy = shotCharge >= 0.f ? currentEnergy - sqrt(shotCharge) : currentEnergy;
	const float value = std::ceilf(energy * 100) / 100;
	Text txt_load(Assets::font_30);
	txt_load.setString(std::to_string(value));
	Window::Draw(txt_load, pos)
		.withOrigin(txt_load.getSize().x, 0)
		.withScale(0.5f);
}
