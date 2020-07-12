
#include "player.h"
#include "planet.h"
#include "input.h"
#include "assets.h"
#include "animation.h"
#include "window.h"
#include "rand.h"
#include "asteroid.h"
#include "text.h"
#include "collide.h"

const float distanceFromPlanet = 100.f;
const float shotSpawnDistance = 30.f;

const float cannonMaxAngle = 75.f;

const float shotChargeSpeed = 0.7f;
const float shotMinCharge = 0.2f;
const float shotMaxCharge = 2.5f;

const float cannonVel = 300.f;
const float accel = 1000.f;
const float maxVel = 2000.f;
const float friction = 0.9f;

extern float mainClock;
const float maxEnergy = 1.5 * sqrt(shotMaxCharge);
const float loadTime = 3.f;
const float shieldTime = 5.f;

// Energy/second 
const float chargingRate = .25f;


const SDL_Color p0Color = { 50, 170, 220, 255 };
const SDL_Color p1Color = { 200, 30, 30, 255 };

Player::Player(int id)
	: id(id)
	, angle(id == 0 ? 90.f : 270.f)
	, cannonAngle(id == 0 ? 90.f : 270.f)
	, currentEnergy(maxEnergy),
	asteroidAnim(id == 0 ? AnimLib::ASTERVOID1 : AnimLib::ASTERVOID2)
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

	if (Input::IsPressed(id, GameKeys::LEFT)) {
		angularVel += -accel * dt;
	} 
	else if (Input::IsPressed(id, GameKeys::RIGHT)) {
		angularVel += accel * dt;
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
			//Sound::Stop(soundLoadChannel);
			if (shotCharge > 2.f) {
				Assets::shot_large[Random::roll(std::size(Assets::shot_large))].Play();
			}
			else if (shotCharge > 1.f) {
				Assets::shot_medium[Random::roll(std::size(Assets::shot_medium))].Play();
			}
			else {
				Assets::shot_small[Random::roll(std::size(Assets::shot_small))].Play();
			}
			new Asteroid(id, shotCharge, shotPos, vec::FromAngle(Mates::DegsToRads(cannonAngle)) * 30000);
			currentEnergy -= sqrt(shotCharge);
			if (currentEnergy < 0.f) currentEnergy = 0.f;
			shotCharge = -1.f;
		}

	}
	else if (Input::IsPressed(id, GameKeys::SHOOT)) {
		Debug::out << "IS PRESSED";
		if (currentEnergy > sqrt(shotMinCharge)) {
			//soundLoadChannel = Assets::loadshot.Play();
			shotCharge = 0.f;
		}
	}
	if (currentShieldTime > 0) {
		currentShieldTime -= dt;
	}
	else if (Input::IsJustPressed(id, GameKeys::SHIELD)) {
		currentShieldTime = shieldTime;
		CircleBounds shieldArea(pos, shieldInfluence);
		Assets::shield.Play();
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
			.withOrigin(vec(animRect.w, animRect.h) / 2)
			.withRect(animRect)
			.withScale(1.5f * sqrt(shotCharge));

	}

	if (currentShieldTime <= 0) {
		Window::DrawPrimitive::Circle(pos, shieldInfluence, 1.5f, id == 0 ? p0Color : p1Color);
	}
	else {
		const float currentTime = shieldTime - currentShieldTime;
		const float radius = easeOutQuad(currentTime, 2.f, shieldInfluence, shieldTime);
		Window::DrawPrimitive::Circle(pos, radius, .5f, 90, 90, 90);
	}

	Window::Draw(id == 0? Assets::ship1Texture : Assets::ship2Texture, pos)
		.withRect({fabs(angularVel) > 50.f ? 64.f : 0.f, 0.f, 64.f, 64.f })
		.withScale(angularVel < 50.f ? 1.f : -1.f, 1.f)
		.withOrigin(vec(32.f, 32.f))
		.withRotation(cannonAngle + 90.f);

	const float widthEnergyBar = 65.f;
	const float radiusEnergyBar = 40.f;
	const float energyBarThickness = 3.5f;
	Window::DrawPrimitive::Arc(pos, radiusEnergyBar, cannonAngle + 180 - widthEnergyBar / 2.f, cannonAngle + 180 + widthEnergyBar / 2.f, energyBarThickness, { 255,255,255,255 });
	if (currentEnergy > 0.f) {
		const float energy = shotCharge >= 0.f ? currentEnergy - sqrt(shotCharge) : currentEnergy;
		const float energyRatio = energy / maxEnergy;
		Window::DrawPrimitive::Arc(pos, radiusEnergyBar, cannonAngle + 180 - (energyRatio * widthEnergyBar / 2.f), cannonAngle + 180 + (energyRatio * widthEnergyBar / 2.f), energyBarThickness, id == 0 ? p0Color : p1Color);
	}
	

}
