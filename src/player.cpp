
#include "player.h"

#include "planet.h"
#include "input.h"
#include "assets.h"
#include "animation.h"
#include "window.h"
#include "asteroid.h"

const float distanceFromPlanet = 70.f;
const float shotSpawnDistance = 30.f;

const float cannonMaxAngle = 75.f;

const float shotChargeSpeed = 0.5f;
const float shotMinCharge = 0.1f;
const float shotMaxCharge = 2.5f;

const float cannonVel = 2.f;
const float accel = 0.6f;
const float maxVel = 2.f;
const float friction = 0.75f;

extern float mainClock;

Player::Player(int id, int owner_planet)
	: id(id)
	, owner_planet(owner_planet)
	, angle(90.f)
	, cannonAngle(0.f)
{
}

void Player::Update(float dt)
{
	if (Input::IsReleased(id, GameKeys::LEFT) && Input::IsReleased(id, GameKeys::RIGHT)) {
		//invertControlsX = angle > 180;
	}
	if (Input::IsReleased(id, GameKeys::UP) && Input::IsReleased(id, GameKeys::DOWN)) {
		//invertControlsY = angle > 90 && angle < 270;
	}
	
	if (Input::IsPressed(id, GameKeys::RIGHT)) {
		if (invertControlsX) {
			angularVel += accel;
		}
		else {
			angularVel -= accel;
		}
	} else if (Input::IsPressed(id, GameKeys::LEFT)) {
		if (invertControlsX) {
			angularVel -= accel;
		}
		else {
			angularVel += accel;
		}
	} else if (Input::IsPressed(id, GameKeys::UP)) {
		if (invertControlsY) {
			angularVel += accel;
		}
		else {
			angularVel -= accel;
		}
	} else if (Input::IsPressed(id, GameKeys::DOWN)) {
		if (invertControlsY) {
			angularVel -= accel;
		}
		else {
			angularVel += accel;
		}
	}

	Mates::Clamp(angularVel, -maxVel, maxVel);

	angle += angularVel;
	
	if (angle > 360.f) angle -= 360.f;
	else if (angle < 0.f) angle += 360.f;

	angularVel *= friction;

	pos = planet->pos + vec::FromAngle(Mates::DegsToRads(angle)) * distanceFromPlanet;

	// Cannon
	if (Input::IsPressed(id, GameKeys::CANNON_RIGHT)) {
		if (invertControlsX) {
			cannonAngle += cannonVel;
		}
		else {
			cannonAngle -= cannonVel;
		}
	}
	else if (Input::IsPressed(id, GameKeys::CANNON_LEFT)) {
		if (invertControlsX) {
			cannonAngle -= cannonVel;
		}
		else {
			cannonAngle += cannonVel;
		}
	}
	else if (Input::IsPressed(id, GameKeys::CANNON_UP)) {
		if (invertControlsY) {
			cannonAngle += cannonVel;
		}
		else {
			cannonAngle -= cannonVel;
		}
	}
	else if (Input::IsPressed(id, GameKeys::CANNON_DOWN)) {
		if (invertControlsY) {
			cannonAngle -= cannonVel;
		}
		else {
			cannonAngle += cannonVel;
		}
	}

	if (cannonAngle < -cannonMaxAngle) cannonAngle = -cannonMaxAngle;
	else if (cannonAngle > cannonMaxAngle) cannonAngle = cannonMaxAngle;

	if (shotCharge >= 0.f) {

		shotCharge += shotChargeSpeed * dt;
		if (shotCharge > shotMaxCharge) shotCharge = shotMaxCharge;
		
		shotPos = pos + vec::FromAngle(Mates::DegsToRads(angle + cannonAngle)) * shotSpawnDistance;

		if ((Input::IsReleased(id, GameKeys::SHOOT) && shotCharge > shotMinCharge) || shotCharge >= shotMaxCharge) {
			new Asteroid(shotCharge, shotPos, vec::FromAngle(Mates::DegsToRads(angle + cannonAngle)) * 300);
			shotCharge = -1.f;
		}

	}
	else if (Input::IsJustPressed(id, GameKeys::SHOOT)) {
		shotCharge = 0.f;
	}


	

}

void Player::Draw() const
{
	const GPU_Rect& playerRect = AnimLib::PLAYER;
	Window::Draw(Assets::invadersTexture, pos)
		.withRect(playerRect)
		.withColor(id == 0 ? SDL_Color{0,255,255,255} : SDL_Color{255, 255, 0, 255})
		.withOrigin(vec(playerRect.w / 2, 8))
		.withRotation(angle + 90);

	const GPU_Rect& cannonRect = AnimLib::CANNON;
	Window::Draw(Assets::invadersTexture, pos)
		.withRect(cannonRect)
		.withColor(id == 0 ? SDL_Color{ 0,255,255,255 } : SDL_Color{ 255, 255, 0, 255 })
		.withOrigin(vec(cannonRect.w, cannonRect.h) / 2)
		.withRotation(angle + cannonAngle + 90);


	if (shotCharge > 0.f) {
		const GPU_Rect& asteroidRect = AnimLib::ASTEROID;
		Window::Draw(Assets::invadersTexture, shotPos)
			.withRect(asteroidRect)
			.withScale(sqrt(shotCharge))
			.withOrigin(vec(asteroidRect.w, asteroidRect.h) / 2);
	}
}
