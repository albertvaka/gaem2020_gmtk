
#include "player.h"

#include "input.h"
#include "assets.h"
#include "animation.h"
#include "bullet.h"
#include "window.h"

const float distanceFromPlanet = 200.f;

const float accel = 0.5f;
const float maxVel = 1.f;
const float friction = 0.7f;

Player::Player(int id, vec planet_center) 
	: id(id)
	, planet_center(planet_center)
	, angle(90.f)
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
}

void Player::Draw() const
{
	const GPU_Rect& animRect = AnimLib::PLAYER;
	Window::Draw(Assets::invadersTexture, pos)
		.withRect(animRect)
		.withColor({0,255,255,255})
		.withOrigin(vec(animRect.w, 0)/2)
		.withRotation(Camera::GetCenter().Angle(pos) + 90);
}
