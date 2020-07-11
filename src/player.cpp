
#include "player.h"
#include "planet.h"
#include "input.h"
#include "assets.h"
#include "animation.h"
#include "window.h"
#include "asteroid.h"
#include "text.h"
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
const int maxLoad = 4;
const float loadTime = 3.f;
const float shieldTime = 5.f;
const float shieldInfluence = 200.f;
Player::Player(int id)
	: id(id)
	, angle(90.f)
	, cannonAngle(0.f),
	currentLoad(maxLoad),
	currentLoadTime(loadTime),
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
	if (currentLoad < maxLoad) {
		if (currentLoadTime < 0.0) {
			currentLoad += 1;
			currentLoadTime = loadTime;
		}
		else {
			currentLoadTime -= dt;
		}
	}
	if (shotCharge >= 0.f) {
		asteroidAnim.Update(dt);
		shotCharge += shotChargeSpeed * dt;
		if (shotCharge > shotMaxCharge) shotCharge = shotMaxCharge;
		
		
		shotPos = pos + vec::FromAngle(Mates::DegsToRads(cannonAngle)) * shotSpawnDistance;

		if ((Input::IsReleased(id, GameKeys::SHOOT) && shotCharge > shotMinCharge) || shotCharge >= shotMaxCharge) {
			new Asteroid(shotCharge, shotPos, vec::FromAngle(Mates::DegsToRads(cannonAngle)) * 30000);
			shotCharge = -1.f;
			currentLoad -= 1;
		}

	}
	else if (Input::IsJustPressed(id, GameKeys::SHOOT)) {
		if (currentLoad > 0) {
			shotCharge = 0.f;
		}
	}
	if (currentShieldTime > 0) {
		currentShieldTime -= dt;
	}
	else if (Input::IsJustPressed(id, GameKeys::SHIELD)) {
		currentShieldTime = shieldTime;
		auto asteroids = Asteroid::GetAll();
		for (auto asteroid : asteroids) {
			float dist = (asteroid->pos - pos).Length();
			if (dist < shieldInfluence) {
				vec outwards_dir = (asteroid->pos - pos).Normalized();

				asteroid->velocity += 1000.0f * outwards_dir;
				asteroid->max_speed_mult = 2.5f;
			}
		}
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

	Text txt_load(Assets::font_30);
	txt_load.setString(std::to_string(currentLoad));
	Window::Draw(txt_load, pos)
		.withOrigin(txt_load.getSize().x, 0)
		.withScale(0.5f);

	Text txt_shield(Assets::font_30);
	txt_shield.setString(currentShieldTime > 0 ? "Used" : "Active");
	Window::Draw(txt_shield, pos)
		.withOrigin(txt_shield.getSize().x*2, 0)
		.withScale(0.5f);

	if (Debug::Draw) {
		Window::DrawPrimitive::Circle(pos, shieldInfluence, 3, 0, 255, 255);
	}
}
