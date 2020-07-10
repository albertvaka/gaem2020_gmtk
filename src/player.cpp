
#include "player.h"

#include "input.h"
#include "assets.h"
#include "animation.h"
#include "window.h"
#include "asteroid.h"

const float distanceFromPlanet = 200.f * 0.8f;

const float cannonMaxAngle = 75.f;

const float cannonVel = 2.f;
const float accel = 0.5f;
const float maxVel = 1.f;
const float friction = 0.7f;

Player::Player(int id, vec planet_center)
	: id(id)
	, planet_center(planet_center)
	, angle(90.f)
	, cannonAngle(0.f)
{
}

void Player::Update(float dt)
{
	if (Input::IsReleased(id, GameKeys::LEFT) && Input::IsReleased(id, GameKeys::RIGHT)) {
		invertControlsX = angle > 180;
	}
	if (Input::IsReleased(id, GameKeys::UP) && Input::IsReleased(id, GameKeys::DOWN)) {
		invertControlsY = angle > 90 && angle < 270;
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

	pos = planet_center + vec::FromAngle(Mates::DegsToRads(angle)) * distanceFromPlanet;


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

	if (Input::IsJustPressed(id, GameKeys::SHOOT)) {
		new Asteroid(100, pos, vec::FromAngle(Mates::DegsToRads(angle + cannonAngle))*100);
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
}
